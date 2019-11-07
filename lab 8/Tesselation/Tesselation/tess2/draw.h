
void draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    Program::setUniform("lightPosition",  globs->scene.lightPosition);
    Program::setUniform("lightColor", vec3(1,1,1));
	Program::setUniform("tessFactor", 1);
	globs->scene.camera.setUniforms();
	Program::setUniform("worldMatrix", mat4::identity());
    
    globs->mainProg.use();
    globs->heightmap.bind(1);
    for(auto& M : globs->scene.meshes ){
		M.draw(GL_PATCHES, globs->verticesPerPatch);
    }

    globs->text1.draw();
    globs->text2.draw();
}

