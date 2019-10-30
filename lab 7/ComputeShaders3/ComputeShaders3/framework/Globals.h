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
	alignas(4) uint32_t rayCount = 0, arraySize = 0, padding[2];
	alignas(16) GPURay rays[];
};

/*Creates a new Vector with a single rayBuffer inside
	The RayBuffer is initialized with raySize of num pixels on window to draw,
	and all vectors set to 0
*/
std::vector<rayBuff>* makeVec_rayBuff(int winWidth, int winHeight)
{
	std::vector<rayBuff>* tmp = new std::vector<rayBuff>();

	unsigned arraySize = winWidth * winHeight;
	
	rayBuff* r = (rayBuff*)malloc(sizeof(uint32_t)*4 + sizeof(GPURay) * arraySize);

	std::cout << "\nsizeof GPURay = " << sizeof(GPURay) << std::endl;
	std::cout << "RayBuffSize: " << sizeof(*r) << " sizeof 4*uint32_t+sizeof(GPURay)*arraySize = " << sizeof(uint32_t) * 4 + sizeof(GPURay) * arraySize << "\n" <<std::endl;
	std::cout << "ArraySize: " << arraySize << std::endl;
	//std::cout << "ArrayLength" << sizeof(*r->rays) << std::endl;

	r->rayCount = 0;
	r->arraySize = arraySize;
	r->padding[0] = 0; r->padding[1] = 0;
	for (int i = 0; i < arraySize; i++)
	{
		r->rays[i].accumulatedColor = vec4(0, 0, 0, 0);
		r->rays[i].pixelCoords = vec4(0, 0, 0, 0);
		r->rays[i].rayDir = vec4(0, 0, 0, 0);
		r->rays[i].rayStartd = vec4(0, 0, 0, 0);
	}

	tmp->push_back(*r);
	
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
    Scene scene{"scene6.txt"};
    std::set<int> keyset;

    bool paused=false, allowReflections = true;
	unsigned reflectionPasses = 2;
	std::shared_ptr<Buffer> sphereBuffer, triangleBuffer, rayBufferA, rayBufferB;
};
