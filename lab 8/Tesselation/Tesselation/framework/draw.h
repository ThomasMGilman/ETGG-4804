void draw2()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Program::setUniform("lightPosition", globs->scene.lightPosition);
	Program::setUniform("lightColor", vec3(1, 1, 1));
	Program::setUniform("reflections", globs->allowReflections);		//enables or disables reflections
	globs->scene.camera.setUniforms();

	//globs->mainProg.use();
	//Program::setUniform("worldMatrix", mat4::identity());
	//for (auto& M : globs->scene.meshes) {
		//M.draw();
	//}

	globs->sphereBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 0);		//Bind SphereBuffer
	globs->triangleBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 1);	//Bind TriangleBuffer
	
}

void doReflections()
{
	//std::cout << "\ndoing reflections\n" << std::endl;
	uint32_t zero[] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
	for (int i = 0; i < globs->reflectionPasses; i++)
	{
		//std::cout << (i == 0) << " FirstPass Index: " << i << std::endl;
		//std::cout << (i == globs->reflectionPasses - 1) << " LastPass Index: " << i << " numPasses: " << globs->reflectionPasses << std::endl;
		Program::setUniform("firstPass", (i == 0));
		Program::setUniform("lastPass", (i == globs->reflectionPasses - 1));
		Program::updateUniforms();

		globs->rayBufferA->bindBase(GL_SHADER_STORAGE_BUFFER, 2);	//currentRays
		globs->rayBufferB->bindBase(GL_SHADER_STORAGE_BUFFER, 3);	//nextPassRays


		glClearBufferSubData(GL_SHADER_STORAGE_BUFFER,				//Buffer to Clear
								GL_R32UI,							//internal format of buffer
								0,									//offset into Buffer to start filling
								4,									//Size to fill in bytes
								GL_RED_INTEGER,						//format of data to put
								GL_UNSIGNED_INT,					//data type
								zero);								//data to fill in buffer to clear

		globs->cs.dispatch(globs->fbo->w / 32, globs->fbo->h, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		glFinish();


		rayBuff* rb = (rayBuff*)globs->rayBufferB->ptr;
		rayBuff* ra = (rayBuff*)globs->rayBufferA->ptr;
		swap(globs->rayBufferA, globs->rayBufferB);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		glFinish();
		
	}
}


void draw(){
	globs->fbo->setAsRenderTarget(false);
	draw2();
	globs->fbo->unsetAsRenderTarget();

    globs->cs.use();
    globs->scene.camera.setUniforms();
    Program::updateUniforms();
    globs->fbo->texture->bindImage(0);

	if (globs->allowReflections)
		doReflections();
	else
		globs->cs.dispatch(globs->fbo->w / 32, globs->fbo->h, 1);
		

    globs->fbo->texture->unbindImage(0);
    glMemoryBarrier( GL_ALL_BARRIER_BITS );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glBindFramebuffer( GL_READ_FRAMEBUFFER, globs->fbo->fbo );
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
    glBlitFramebuffer(0,0,globs->fbo->w,globs->fbo->h,
                      0,0,globs->fbo->w,globs->fbo->h,
                      GL_COLOR_BUFFER_BIT,
                      GL_NEAREST );
    glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
}
