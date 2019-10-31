#pragma once 

#include "utils.h"
#include "Mesh.h"
#include "Text.h"
#include "Framebuffer.h"
#include "UnitSquare.h" 
#include "Scene.h"

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
    Program mainProg{"vs.txt","fs.txt"};
    Program cs{"cs.txt"};
    std::shared_ptr<Framebuffer> fbo;
    Sampler mipsampler{true};
    Sampler linearsampler{false};
    Text text1{ vec2(10,35), ""};
    Text text2{ vec2(10,10), ""};
    Scene scene{"scene1.txt"};
    std::set<int> keyset;
	
	bool allowReflections = true;
	bool paused = false;
	unsigned reflectionPasses = 2;				//Vars To control Reflections
	unsigned minReflectionPasses = 2;
	unsigned maxReflectionPasses = 6;
	std::shared_ptr<Buffer> sphereBuffer, triangleBuffer, rayBufferA, rayBufferB;

	ImageTexture2DArray heightmap{ "heightmap.png" };
};
