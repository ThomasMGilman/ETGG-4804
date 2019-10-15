
struct GPUSphere
{
	alignas(16) vec4 centerAndRadius, color;
};

struct GPUTriangle
{

};

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
	//std::vector<>
	for (unsigned i = 0; i < globs->scene.spheres.size(); i++)
	{
		Sphere* s = &globs->scene.spheres[i];
		sphereData[i].centerAndRadius = vec4(s->c, s->r);
		sphereData[i].color = vec4(s->color, 1.0);
	}
	globs->sphereBuffer = Buffer::create(sphereData);
	globs->sphereBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 0);

	Program::setUniform("lightPosition", globs->scene.lightPosition);
	Program::setUniform("lightColor", vec3(1, 1, 1));
}
