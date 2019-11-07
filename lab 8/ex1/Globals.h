#pragma once 

#include "utils.h"
#include "Mesh.h"
#include "Text.h"
#include "Framebuffer.h"
#include "UnitSquare.h" 
#include "Scene.h"
#include "PatchMesh.h"
#include "Camera.h"

class Globals {
    public:
    Program mainProg{"vs.txt","tcs.txt","tes.txt","fs.txt"};
    Sampler mipsampler{true};
    Sampler linearsampler{false};
    Text text1{ vec2(10,35), ""};
    Text text2{ vec2(10,10), ""};
    Text text3{ vec2(0,490), "F1 or F2 changes tess level. F3 toggles wire" };
    Text text4{ vec2(0,470), "Turn: AD Walk: WS Strafe: IJKL Tilt: RF" };
    Text text5{ vec2(0,450), "Tess level: 1"} ;
    PatchMesh pmesh{"goblet.obj.mesh"};
    Camera camera{ vec3(0,0,10), vec3(0,0,0), vec3(0,1,0) };
    vec3 lightPosition{5,10,5};
    std::set<int> keyset;
    bool paused=false;
    float subdivs=1.0f;
    bool wireframe=false;
};
