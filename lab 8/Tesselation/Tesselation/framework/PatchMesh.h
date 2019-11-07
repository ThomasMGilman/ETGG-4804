#pragma once 

#include "Program.h"
#include "ImageTexture2DArray.h"
#include "math3d.h"
#include <string>
#include <vector>
#include <utility>

struct PatchMesh{
    std::string filename;
    int numpatches;
    GLuint vao;
    std::shared_ptr<Buffer> pbuff;
    
    PatchMesh(std::string filename){
        using namespace std;
        int numfloats;
        
        this->filename = filename;
        
        ifstream fp(filename,ios::binary);
        if(!fp)
            throw runtime_error("Cannot open file "+filename);
            
        string line;
        
        getline(fp,line);
        
        if(line != "patches_01" )
            throw runtime_error("Incorrect mesh format: "+line+": Expected patches_01");
            
        GLuint tmp[1];
        glGenVertexArrays(1,tmp);
        this->vao = tmp[0];
        glBindVertexArray(this->vao);
        
        while(true){
            unsigned pos;
            pos = fp.tellg();
            getline(fp,line);
            if( fp.fail() )
                throw runtime_error("Unexpected end of file in "+filename);

            istringstream iss(line);
            string word;
            iss >> word;

            if( iss.fail() ){
            }
            else if( word == "num_floats" ){
                iss >> numfloats;
                this->numpatches = numfloats/3/16;
                assert( numfloats % (3*16) == 0 );
            }
            else if( word == "positions"){
                vector<float> pdata;
                //16 vec3's per patch; 4 bytes per float; 3 floats per vec3
                pdata.resize(numfloats);
                fp.read((char*)pdata.data(),pdata.size()*sizeof(pdata[0]));
                pbuff = Buffer::create(pdata);
                pbuff->bind(GL_ARRAY_BUFFER);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0,3,GL_FLOAT,false,3*4,0);
            }
            else if( word == "material" ){
            }
            else if( word == "end")
                break;
            else{
                cerr << pos << "\n";
                cerr << word << "\n";
                assert(0);
            }
        }
 
        glBindVertexArray(0);
    }

    void drawPatches(){
        glBindVertexArray(this->vao);
        glPatchParameteri(GL_PATCH_VERTICES,16);
        glDrawArrays(GL_PATCHES, 0, numpatches*16);
    }
};
        
