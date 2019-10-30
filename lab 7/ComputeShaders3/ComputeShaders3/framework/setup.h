
void setup(int winwidth, int winheight){
    
    globs = std::make_unique<Globals>();
    globs->fbo = std::make_shared<Framebuffer>(winwidth,winheight,1,GL_RGBA);
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.8f,0.4f,0.6f,1.0f);
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

	//If Reflections enabled, create buffers for doing reflection loop
	if (globs->allowReflections)
	{
		uint32_t sizeOfRayBuff = sizeof(uint32_t) * 4 + sizeof(GPURay) * winwidth * winheight;
		globs->rayBufferA = Buffer::createMappable(sizeOfRayBuff);
		globs->rayBufferB = Buffer::createMappable(sizeOfRayBuff);
	}

	Program::setUniform("lightPosition", globs->scene.lightPosition);
	Program::setUniform("lightColor", vec3(1, 1, 1));
}
