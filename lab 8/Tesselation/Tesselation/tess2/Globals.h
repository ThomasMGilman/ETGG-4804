#pragma once 

#include "utils.h"
#include "Mesh.h"
#include "PatchMesh.h"
#include "Text.h"
#include "Framebuffer.h"
#include "UnitSquare.h" 
#include "Scene.h"
#include "ImageTexture2DArray.h"

class Globals {
    public:
	/////////////////////////////////////////////////////////////////////////////////////////////////Program to use, and sampler
	Program mainProg{ "vs.txt", "fs.txt" };
    Program tessProg{"vs.txt", "tcs.txt", "tes.txt", "fs.txt"};
    Sampler mipsampler{true};
    Sampler linearsampler{false};

	/////////////////////////////////////////////////////////////////////////////////////////////////Text To Draw
	Text text1{ vec2(10,35), "" };
	Text text2{ vec2(10,10), "" };
	Text text3{ vec2(0,490), "F1 or F2 changes tess level. F3 toggles wire" };
	Text text4{ vec2(0,470), "Turn: AD Walk: WS Strafe: IJKL Tilt: RF" };
	Text text5{ vec2(0,450), "Tess level: 1" };

	/////////////////////////////////////////////////////////////////////////////////////////////////Scene to Draw
    Scene scene{"scene1.txt"};

	PatchMesh pmesh{ "goblet.obj.mesh" };

	/////////////////////////////////////////////////////////////////////////////////////////////////Global Variables
    std::set<int> keyset;
    bool paused=false;
	/////////////////////////////////////////////////////////////Tesselation Variables
	int verticesPerPatch = 3;
	float subdivs = 1.0f;
	bool wireframe = false;

	/////////////////////////////////////////////////////////////////////////////////////////////////Image to draw on obj
    ImageTexture2DArray heightmap{"heightmap.png"};
};
