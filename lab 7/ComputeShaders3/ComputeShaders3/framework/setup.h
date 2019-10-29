
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
	alignas(4) int rayCount = 0, arraySize = 0;
	alignas(16) GPURay *rays;
};

std::vector<rayBuff> makeVec_rayBuff(int winWidth, int winHeight)
{
	std::vector<rayBuff> *tmp = new std::vector<rayBuff>();
	rayBuff* r = new rayBuff;
	unsigned arraySize = winWidth * winHeight;
	r->rayCount = 0; r->rays = new GPURay[arraySize];
	r->arraySize = arraySize;
	tmp->push_back(*r);
	std::cout << "ArraySize: " << arraySize << std::endl;
	return *tmp;
}

void setup(int winwidth, int winheight){
    
    globs = std::make_unique<Globals>();
    globs->fbo = std::make_shared<Framebuffer>(winwidth,winheight,1,GL_RGBA);
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.2f,0.4f,0.6f,1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    globs->mipsampler.bind(0);
    globs->linearsampler.bind(1);
	std::vector<GPUSphere> sphereData(globs->scene.spheres.size());

	for (unsigned i = 0; i < globs->scene.spheres.size(); i++)
	{
		Sphere* s = &globs->scene.spheres[i];
		sphereData[i].center_radius = vec4(s->c, s->r);
		sphereData[i].color			= vec4(s->color, s->alpha);
		sphereData[i].reflection	= vec4(s->refl, 0, 0, 0);
	}
	globs->sphereBuffer = Buffer::create(sphereData);
	globs->sphereBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 0);

	int numTriangles = 0;
	for (Mesh& m : globs->scene.meshes)
		numTriangles += m.numTriangles;

	std::vector<GPUTriangle> triangleData(numTriangles);
	unsigned triangleOffset = 0;
	for (Mesh& m : globs->scene.meshes)
	{
		for (unsigned i = 0; i < m.numTriangles; i++)
		{
			Triangle* t = &m.triangles[i];
			triangleData[triangleOffset + i].color				= vec4(m.color, 0);
			triangleData[triangleOffset + i].D_oneOverTwiceArea = vec4(t->D, t->oneOverTwiceArea, 0, 0);
			triangleData[triangleOffset + i].N_reflection		= vec4(t->N, m.refl);
			for (unsigned j = 0; j < 3; j++)
			{
				triangleData[triangleOffset + i].point[j]	= vec4(t->p[j], 0);
				triangleData[triangleOffset + i].edge[j]	= vec4(t->e[j], 0);
			}
		}
		triangleOffset += m.numTriangles;
	}
	globs->triangleBuffer = Buffer::create(triangleData);
	globs->triangleBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 1);

	std::vector<rayBuff> rayBuffA = makeVec_rayBuff(winwidth, winheight); //make sure to destroy at end
	std::vector<rayBuff> rayBuffB = makeVec_rayBuff(winwidth, winheight);
	globs->rayBufferA = Buffer::createMappable(rayBuffA);
	globs->rayBufferB = Buffer::createMappable(rayBuffB);

	//std::cout << "\n" << sqrt(15 ^ 2) << "\n" <<std::endl;

	Program::setUniform("lightPosition", globs->scene.lightPosition);
	Program::setUniform("lightColor", vec3(1, 1, 1));
}
