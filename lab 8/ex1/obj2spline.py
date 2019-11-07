#!/usr/bin/env python3

import sys
import array
from math3d import *
try:
    import tkinter.filedialog
except ImportError:
    pass


def main():
            
    #get input and output file names
    if len(sys.argv) == 1:
        infile = tkinter.filedialog.askopenfilename()
        if not infile:
            return
        convert(infile)
    else:
        for infile in sys.argv[1:]:
            convert(infile)


def convert(infile):
    outfile = infile+".mesh"
    
    #vertex data will be a list of vec3's
    vertexdata = [] 
    
    #list of vec2's
    texturedata = [] 
    
    #list of vec3's
    normaldata = []
    
    #key = material name, value = list of triangles. 
    #Each triangle consists of three 
    #(vertex index, texture index, normal index, material index) tuples
    quads = { "":[] } 
    
    #materials. Key = material name; value = dictionary
    mdict = {} 

    #adjacency list graph
    #key = vertex index
    #value = set of vertices it's connected to
    G={}
    
    #key = vertex index, value = name of material
    materials = {}
    
    #current material
    currmtl=""

    for L in open(infile):
        L=L.strip()
        
        if len(L) == 0:
            continue
            
        L = L.split(" ")
        
        if L[0].startswith("#"):
            pass
        elif L[0] == "o" :
            currobj = L[1]
        elif L[0] == "v" :
            #a vertex (xyz) specification
            pt = [float(q) for q in L[1:]]
            vertexdata.append(vec3(pt[0],pt[1],pt[2]))
        elif L[0] == "vt" :
            #texture coordinate
            pt = [float(q) for q in L[1:]]
            texturedata.append(vec2(pt[0],pt[1]))
        elif L[0] == "vn":
            #normal
            pt = [float(q) for q in L[1:]]
            normaldata.append( vec3(pt[0],pt[1],pt[2]) )
        elif L[0] == "f" :
            #face 
            
            V = L[1:]
            if len(V) != 4:
                print("Error: I only support quads.")
                assert(0)
            
            #t=the current quad
            t=[] 
            for vspec in V:
                #four formats possible: vi  vi//ni  vi/ti  vi/ti/ni
                tmp = vspec.split("/") 
                
                #obj uses one-based indexing, so we must subtract one here
                vi=int(tmp[0])-1 
                
                #if no texture coordinate, make one up
                if len(tmp) < 2 or len(tmp[1]) == 0:
                    ti=0
                else:
                    ti = int(tmp[1])-1 
                    
                if len(tmp) < 3 or len(tmp[2]) == 0:
                    ni=0
                else:
                    ni = int(tmp[2])-1
                    
                t.append( (vi,ti,ni) )
            
            assert len(t) == 4
            for i in range(len(t)):
                v,_,_ = t[i]
                w,_,_ = t[ (i+1)%4 ]
                u,_,_ = t[ (i+3)%4 ]
                
                if v not in G:
                    G[v] = set()
                if w not in G:
                    G[w] = set()
                if u not in G:
                    G[u] = set()
                    
                G[v].add(w)
                G[v].add(u)
                
                if v not in materials:
                    materials[v] = currmtl
                    
                
            quads[currmtl].append(t) 
            
        elif L[0] == "mtllib" :
            #material library; must parse it
            ML = open(L[1]).readlines()
            
            #look at each material and store information about it
            for m in ML:
                m=m.strip()
                if len(m) == 0:
                    continue
                    
                #important: only split once in case we have a
                #parameter with a space in it
                m=m.strip().split(" ",1)
                
                if m[0].startswith("#"):
                    pass
                elif m[0] == "newmtl" :
                    mname = m[1] 
                    mdict[mname]={"count":0}
                else:
                    mdict[mname][m[0]] = m[1]
        elif L[0] == "usemtl" :
            #change currently active material
            currmtl = L[1]
            if currmtl not in quads:
                quads[currmtl]=[]
        elif L[0] == 'l' or L[0] == 's':
            pass
        else:
            print("Note: Skipping",L)
    
    
    
    #now we want to determine our patches
    #we'll accomplish this via a depth-first walk of the vertices
    cc=[]      #list of connected components. 
    
    
       
    def dfs(G,vi,visited,P):
        assert vi not in visited
        visited.add(vi)
        P.add(vi)
        for n in G[vi]:
            if n not in visited:
                dfs(G,n,visited,P)

    #list of vertex indices (indices in vertexdata)
    visited=set()
    for vi in G:
        if vi not in visited:
            P=set()
            dfs(G,vi,visited,P)
            if len(P) != 16:
                print("Patch without 16 vertices")
                print(len(P))
                print(P)
                assert 0
            cc.append(P)
             
         
    def corner(G, v1, v2, exclude):
        #return common neighbor of v1 and v2 that is not exclude
        #  excl v1
        #   +---+
        #   |   |
        #   +---+
        #  v2   ^returned
        common = G[v1] & G[v2]
        common.remove(exclude)
        assert len(common) == 1
        return list(common)[0]
    
    def single(G,v1,excl1, excl2):
        #return the only neighbor of v1 that is not excl1 or excl2
        tmp = set(G[v1])
        tmp.remove(excl1)
        tmp.remove(excl2)
        assert len(tmp) == 1
        return list(tmp)[0]
        
    #keyed by material
    patches={}
    numpatches=0
    
    #for each patch, we want to convert to 4x4 grid
    for P in cc:
        #choose a corner. This will be a vertex with only two incident edges
        Q=[]
        for i in range(4):
            Q.append([])
            for j in range(4):
                Q[-1].append(None)
                
        for v in P:
            if len(G[v]) == 2:
                #this is our corner
                Q[0][0] = v
                tmp = list(G[v])
                Q[0][1] = tmp[0]
                Q[1][0] = tmp[1]
                Q[1][1] = corner(G,Q[0][1],Q[1][0],Q[0][0])
                Q[0][2] = single(G,Q[0][1],Q[1][1],Q[0][0])
                Q[2][0] = single(G,Q[1][0],Q[0][0],Q[1][1])
                Q[1][2] = corner(G,Q[0][2],Q[1][1],Q[0][1])
                Q[2][1] = corner(G,Q[1][1],Q[2][0],Q[1][0])
                Q[2][2] = corner(G,Q[1][2],Q[2][1],Q[1][1])
                Q[0][3] = single(G,Q[0][2],Q[1][2],Q[0][1])
                Q[1][3] = corner(G,Q[0][3],Q[1][2],Q[0][2])
                Q[2][3] = corner(G,Q[1][3],Q[2][2],Q[1][2])
                Q[3][0] = single(G,Q[2][0],Q[1][0],Q[2][1])
                Q[3][1] = corner(G,Q[2][1],Q[3][0],Q[2][0])
                Q[3][2] = corner(G,Q[2][2],Q[3][1],Q[2][1])
                Q[3][3] = corner(G,Q[2][3],Q[3][2],Q[2][2])
                break
        else:
            assert 0
            
        mname = materials[Q[0][0]]
        if mname not in patches:
            patches[mname]=[]
            
        patches[mname].append(Q)
        numpatches+=1
        
     
        
    ofp = open(outfile,"wb")
    ofp.write(b"patches_01\n") 
    
       
    tmp=[]
  
    #sort by material
    mm = sorted(patches.keys())
    for mname in mm:
        patchlist = patches[mname]
        for P in patchlist:
            for row in range(4):
                for col in range(4):
                    vi=P[row][col]
                    v=vertexdata[vi]
                    #print(v)
                    tmp.append(v.x)
                    tmp.append(v.y)
                    tmp.append(v.z)
                    
    ofp.write( ("num_floats "+str(len(tmp))+"\n").encode() )
   
    
    # ~ #give vertex ranges for each material
    # ~ start=0
    # ~ for mtlname in mm:
        # ~ numpatches = len(patches[mname])
        # ~ num = 16*numpatches     #number of vec3s
        # ~ ofp.write( ("material %d %d %s %s" % (start,num,mtlname,mdict[mtlname]["Kd"]) ).encode() )
        # ~ for k in mdict[mtlname]:
            # ~ if k.startswith("map_") or k == "refl":
                # ~ ofp.write( (" "+k+" "+mdict[mtlname][k]).encode() )
        # ~ ofp.write(b"\n")
        # ~ start += num
        
    #positions
    A = array.array("f",tmp)
    assert len(tmp) == numpatches*16*3
    ofp.write(b"positions\n")
    ofp.write(A.tobytes())
    ofp.write(b"\n")
       
     
      
       
    ofp.write(b"\nend\n") 
    ofp.close()

    return
    
    
    
    
    
    
    
    
    
    
    # ~ #first, determine how many unique vertices we'll have
    # ~ vmap={}     #key=vi,ti,ni  Value=index in vdata
    # ~ numv=0
    # ~ vdata=[]
    # ~ tdata=[]
    # ~ ndata=[]
    # ~ idata=[]   
    
    # ~ #just for informational purposes
    # ~ numshared=0
    
    # ~ for mtlname in triangles:
        # ~ for T in triangles[mtlname]:
            # ~ #T will be a list of three vi,ti,ni tuples
            # ~ assert len(T) == 3
            # ~ for vi,ti,ni in T:
                # ~ key = (vi,ti,ni)
                # ~ if key not in vmap:
                    # ~ #first time we've seen this vertex, so add it
                    # ~ #to our vertex list and our dictionary
                    # ~ vmap[key]=numv
                    # ~ numv+=1
                    # ~ vdata += [vertexdata[vi][0], vertexdata[vi][1], vertexdata[vi][2] ]
                    # ~ tdata += [texturedata[ti][0], texturedata[ti][1] ]
                    # ~ ndata += [normaldata[ni][0],normaldata[ni][1], normaldata[ni][2]]
                # ~ else:
                    # ~ numshared+=1  #only for curiosity :-)
                    
                # ~ idata.append( vmap[key] )
    
    # ~ #information for the user
    # ~ print(len(vdata)//3,"vertices") 
    # ~ print(len(idata),"indices")
    # ~ print(numshared,"shared")
    
    # ~ ofp = open(outfile,"wb")
    # ~ ofp.write(b"mesh_01s\n") 
    # ~ ofp.write( ("num_vertices "+str(len(vdata)//3)+"\n").encode() ) 
    # ~ ofp.write( ("num_indices "+str(len(idata))+"\n").encode() ) 

    # ~ #positions
    # ~ tmp = array.array("f",vdata)
    # ~ ofp.write(b"positions\n")
    # ~ ofp.write(tmp.tobytes())
    # ~ ofp.write(b"\n")
    
    # ~ #texture coordinates
    # ~ tmp = array.array("f",tdata)
    # ~ ofp.write(b"texcoords\n")
    # ~ ofp.write(tmp.tobytes())
    # ~ ofp.write(b"\n")

    # ~ #normals
    # ~ tmp = array.array("f",ndata)
    # ~ ofp.write(b"normals\n")
    # ~ ofp.write(tmp.tobytes())
    # ~ ofp.write(b"\n")

    # ~ #always 4 bytes per index
    # ~ tmp = array.array("I",idata)
    # ~ ofp.write(b"indices\n")
    # ~ ofp.write(tmp.tobytes()) 
    # ~ ofp.write(b"\n")
    

main()
