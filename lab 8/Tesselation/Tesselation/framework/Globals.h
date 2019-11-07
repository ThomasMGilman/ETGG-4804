#pragma once 

#include "utils.h"
#include "Mesh.h"
#include "Text.h"
#include "Framebuffer.h"
#include "UnitSquare.h" 
#include "Scene.h"
#include "PatchMesh.h"

struct GPURay
{
	alignas(16) vec4 rayStartd, rayDir, pixelCoords, accumulatedColor;
};

struct GPUSphere
{
	alignas(16) vec4 center_radius, color, reflection;
};

struct GPUTriangle
{
	alignas(16) vec4 point[3], edge[3];
	alignas(16) vec4 color, N_reflection, D_oneOverTwiceArea;
};

struct rayBuff
{
	alignas(4) uint32_t rayCount = 0, padding[3]{0,0,0};
	alignas(16) GPURay rays[1];
};

/*Creates a new Vector with a single rayBuffer inside
	The RayBuffer is initialized with raySize of num pixels on window to draw,
	and all vectors set to 0
*/
std::vector<rayBuff> makeVec_rayBuff()
{
	std::vector<rayBuff> tmp = std::vector<rayBuff>();
	
	rayBuff r = rayBuff();
	tmp.push_back(r);
	
	return tmp;
}

class Globals {
    public:
	///////////////////////////////////////////////////////////////////////////////////////// Shaders to use
    Program mainProg{"vs.txt","tcs.txt","tes.txt","fs.txt"};
    Program cs{"cs.txt"};

	///////////////////////////////////////////////////////////////////////////////////////// Shared framebuffer, mipSampler, and linearSampler enables
    std::shared_ptr<Framebuffer> fbo;
    Sampler mipsampler{true};
    Sampler linearsampler{false};

	///////////////////////////////////////////////////////////////////////////////////////// Text to draw on screen
    Text text1{ vec2(10,35), ""};
    Text text2{ vec2(10,10), ""};
	Text text3{ vec2(0,490), "F1 or F2 changes tess level. F3 toggles wire." };
	Text text4{ vec2(0,470), "R toggles reflections, 1 or 2 controls the amount." };
	Text text5{ vec2(0,450), "Turn: Left,Right,J,L Walk: WS" };
	Text text6{ vec2(0,430), "Strafe: Space,LCTRL,A,D Tilt: UP,DOWN" };
	Text text7{ vec2(0,410), "Tess level: 1" };

	//////////////////////////////////////////////////////////////////////////////////////// Scene To Draw, must be included in working directory
    Scene scene{"scene1.txt"};

	//////////////////////////////////////////////////////////////////////////////////////// Program Variables
    std::set<int> keyset;								//Keys pressed
	bool allowReflections = true;						//Enable/Disable Reflections
	bool paused = false;								//Pause/unPause updating scene
	bool wireframe = false;								//Enable/Disable the wireframe of the objects shown
	unsigned reflectionPasses = 2;						//Number of times to reflect off the reflective objects in the scene
	unsigned minReflectionPasses = 2;					//min number of reflections before disabling reflections
	unsigned maxReflectionPasses = 6;					//max number of reflections
	float subdivs = 1.0f;								//Used for tesselation

	//////////////////////////////////////////////////////////////////////////////////////// Buffers used by compute shader
	std::shared_ptr<Buffer> sphereBuffer, triangleBuffer, rayBufferA, rayBufferB;

	//////////////////////////////////////////////////////////////////////////////////////// HeightMap and other images
	ImageTexture2DArray heightmap{ "heightmap.png" };

};
