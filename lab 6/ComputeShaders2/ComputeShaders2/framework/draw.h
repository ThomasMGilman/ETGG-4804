void draw2()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Program::setUniform("lightPosition", globs->scene.lightPosition);
	Program::setUniform("lightColor", vec3(1, 1, 1));
	globs->scene.camera.setUniforms();
	globs->mainProg.use();
	Program::setUniform("worldMatrix", mat4::identity());
	for (auto& M : globs->scene.meshes) {
		M.draw();
	}

	globs->sphereBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 0);		//Bind SphereBuffer
	globs->triangleBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 1);	//Bind TriangleBuffer
}


void draw(){
	globs->fbo->setAsRenderTarget(false);
	draw2();
	globs->fbo->unsetAsRenderTarget();

    globs->cs.use();
    globs->scene.camera.setUniforms();
    Program::updateUniforms();
    globs->fbo->texture->bindImage(0);
    globs->cs.dispatch(globs->fbo->w/32, globs->fbo->h, 1 );
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
