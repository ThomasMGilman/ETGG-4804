#!/usr/bin/env python3

#jh fall 2019

import sys
import pyavx

TEX=False

counter=0
class mm:
    def __init__(self,sz):
        self.data=[]
        global counter
        counter+=1
    def append(self,x):
        self.data.append(x)
    def __str__(self):
        L=[]
        if TEX:
            L.append(r"\scalebox{0.65}[1.0]{\fontsize{9}{9}\selectfont{ $ ")
        L.append("[")
        for i in range(sz-1,-1,-1):
            if not TEX:
                L.append( "%-3s" % str(self[i]) )
            else:
                L.append( " %s " % str(self[i]) )
            if i == 16:
                if not TEX: 
                    L.append(" || ")
                else:
                    #L.append( "\\linebreak")
                    pass
            elif i%4 == 0  and i != 0:
                if not TEX:
                    L.append(" . ")
                else:
                    L.append(" , ")
            elif i != 0:
                if not TEX:
                    L.append(",")
                else:
                    L.append(" , ")
                
        L.append("]")
        if TEX:
            L.append("$ }}")
            L.append("\n")
            
        return "".join(L)
    def __getitem__(self,i):
        return self.data[i]
    def __setitem__(self,i,o):
        self.data[i]=o
        
def ymm(mm):
    def __init__(self):
        super.__init__(32)

def xmm(mm):
    def __init__(self):
        super.__init__(16)
        
        
#p is a list of at least 32 items
def _mm256_lddqu_si256(p):
    print("_mm256_lddqu_si256")
    a=ymm()
    a.data = p[:32]
    assert len(a.data) == 32
    return a
     
def _mm256_shuffle_epi32(a, imm):
    print("_mm256_shuffle_epi32")
    c = ymm()
    low=[]
    hi=[]
    for i in range(4):
        idx = imm & 3
        imm >>= 2
        low += a.data[idx*4:idx*4+4]
        hi += a.data[16+idx*4:16+idx*4+4]
    assert len(low) == 16
    assert len(hi) == 16
    c.data += low
    c.data += hi
    return c
    
def _mm256_permutevar8x32_epi32(a,b):
    print("_mm256_permutevar8x32_epi32")
    assert type(b) == list
    assert len(b) == 8
    c=ymm()
    for i in range(8):
        idx = b[i]
        assert idx >= 0 and idx <= 7
        c.data += a[idx*4:idx*4+4]
    return c
   
def _mm256_blend_epi32(a,b, imm):
    print("_mm256_blend_epi32")
    assert type(imm) == int
    c=ymm()
    for i in range(8):
        if (1<<i) & imm:
            c.data += (b[i*4:i*4+4])
        else:
            c.data += (a[i*4:i*4+4])
    return c
    
def _mm256_unpacklo_epi8(a,b):
    print("_mm256_unpacklo_epi8")
    c=ymm()
    for i in range(8):
        c.append( a[i] )
        c.append( b[i] )
    for i in range(8):
        c.append( a[16+i] )
        c.append( b[16+i] )
    return c
   
def _mm256_unpacklo_epi16(a,b):
    print("_mm256_unpacklo_epi16")
    c=ymm()
    for i in range(4):
        c.append( a[2*i] )
        c.append( a[2*i+1] )
        c.append( b[2*i] )
        c.append( b[2*i+1] )
    for i in range(4):
        c.append( a[16+2*i] )
        c.append( a[16+2*i+1] )
        c.append( b[16+2*i] )
        c.append( b[16+2*i+1] )
    return c
    
def _mm256_unpacklo_epi32(a,b):
    print("_mm256_unpacklo_epi32")
    c=ymm()
    for i in range(2):
        c.append( a[4*i] )
        c.append( a[4*i+1] )
        c.append( a[4*i+2] )
        c.append( a[4*i+3] )
        c.append( b[4*i] )
        c.append( b[4*i+1] )
        c.append( b[4*i+2] )
        c.append( b[4*i+3] )
    for i in range(2):
        c.append( a[16+4*i] )
        c.append( a[16+4*i+1] )
        c.append( a[16+4*i+2] )
        c.append( a[16+4*i+3] )
        c.append( b[16+4*i] )
        c.append( b[16+4*i+1] )
        c.append( b[16+4*i+2] )
        c.append( b[16+4*i+3] )
    return c
    
def _mm256_unpackhi_epi32(a,b):
    print("_mm256_unpackhi_epi32")
    c=ymm()
    assert type(a) == ymm
    assert type(b) == ymm
    for i in range(2):
        c.append( a[8+4*i] )
        c.append( a[8+4*i+1] )
        c.append( a[8+4*i+2] )
        c.append( a[8+4*i+3] )
        c.append( b[8+4*i] )
        c.append( b[8+4*i+1] )
        c.append( b[8+4*i+2] )
        c.append( b[8+4*i+3] )
    for i in range(2):
        c.append( a[8+16+4*i] )
        c.append( a[8+16+4*i+1] )
        c.append( a[8+16+4*i+2] )
        c.append( a[8+16+4*i+3] )
        c.append( b[8+16+4*i] )
        c.append( b[8+16+4*i+1] )
        c.append( b[8+16+4*i+2] )
        c.append( b[8+16+4*i+3] )
    return c

def _mm256_unpacklo_epi64(a,b):
    print("_mm256_unpacklo_epi64")
    c=ymm()
    c.data += a.data[0:8]
    c.data += b.data[0:8]
    c.data += a.data[16:24]
    c.data += b.data[16:24]
    return c
    
def _mm256_unpackhi_epi64(a,b):
    print("_mm256_unpackhi_epi64")
    c=ymm()
    c.data += a.data[8:16]
    c.data += b.data[8:16]
    c.data += a.data[24:32]
    c.data += b.data[24:32]
    return c
    
def _mm256_unpackhi_epi8(a,b):
    print("_mm256_unpackhi_epi8")
    c=ymm()
    for i in range(8):
        c.append( a[8+i] )
        c.append( b[8+i] )
    for i in range(8):
        c.append( a[24+i] )
        c.append( b[24+i] )
    return c

def _mm256_unpackhi_epi16(a,b):
    print("_mm256_unpackhi_epi16")
    c=ymm()
    for i in range(4):
        c.append( a[8+2*i] )
        c.append( a[8+2*i+1] )
        c.append( b[8+2*i] )
        c.append( b[8+2*i+1] )
    for i in range(4):
        c.append( a[24+2*i] )
        c.append( a[24+2*i+1] )
        c.append( b[24+2*i] )
        c.append( b[24+2*i+1] )
    return c
    
def _mm256_shuffle_epi8(a,b):
    print("_mm256_shuffle_epi8")
    c=ymm()
    assert type(b) == list
    assert len(b) == 32
    for i in range(16):
        if b[i] & 0x80:
            c.data.append(0)
        else:
            c.data.append( a[b[i]&0xf] )
    for i in range(16,32):
        if b[i] & 0x80:
            c.data.append(0)
        else:
            c.data.append( a[16+(b[i]&0xf)] )
    return c
    
def _mm256_permute2f128_si256( a,b, ctrl):
    print("_mm256_permute2f128_si256")
    c=ymm()
    def pick( a,b, idx):
        if idx & 0x8:
            return ["0"]*16
        if idx == 0: return a[0:16]
        if idx == 1: return a[16:32]
        if idx == 2: return b[0:16]
        if idx == 3: return b[16:32]
        print(idx)
        assert 0
    c.data += pick( a.data, b.data, (ctrl & 0xf) )
    c.data += pick( a.data, b.data, (ctrl>>4) & 0xf )
    assert len(c.data) == 32
    return c

  
def _mm256_permute4x64_epi64( a, ctrl):
    print("_mm256_permute4x64_si256")
    c=ymm()
    def pick( a, idx):
        idx &= 3
        if idx == 0: return a[0:8]
        if idx == 1: return a[8:16]
        if idx == 2: return a[16:24]
        if idx == 3: return a[24:32]
        print(idx)
        assert 0
    c.data += pick( a.data, ctrl )
    c.data += pick( a.data, ctrl>>2 )
    c.data += pick( a.data, ctrl>>4 )
    c.data += pick( a.data, ctrl>>6 )
    if len(c.data) != 32:
        print("!!!",len(c.data))
    assert len(c.data) == 32
    return c


if __name__ == "__main__":
    #example syntax
    tmp = []
    for i in range(8):
        tmp.append("r{}".format(i))
        tmp.append("g{}".format(i))
        tmp.append("b{}".format(i))
        tmp.append("a{}".format(i))
        
    val0 = pyavx._mm256_lddqu_si256(tmp[0:32])
    print("val0:",val0)
    val1 = pyavx._mm256_permute4x64_si256( val0, 0xab)
    print("val1:",val1)
    
