#pragma once 

#include "utils.h"
#include "Mesh.h"
#include "Text.h"
#include "Framebuffer.h"
#include "UnitSquare.h" 
#include "Scene.h"

class Globals {
    public:
    Program mainProg{"vs.txt","fs.txt"};
	Program cs{ "cs.txt" };
    Sampler mipsampler{true};
    Sampler linearsampler{false};
    Text text1{ vec2(10,35), ""};
    Text text2{ vec2(10,10), ""};
    Scene scene{"scene2.txt"};
    std::set<int> keyset;
	std::shared_ptr<Framebuffer> fbo;
    bool paused=false;
    
};
