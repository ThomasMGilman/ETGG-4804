
void draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    if( globs->wireframe )
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    Program::setUniform("lightPosition",  globs->camera.eye);
    Program::setUniform("lightColor", vec3(1,1,1));
    Program::setUniform("subdivs",globs->subdivs);
    Program::setUniform("shadeless",float(globs->wireframe));
    
    globs->camera.setUniforms();
    globs->mainProg.use();
    Program::setUniform("worldMatrix",mat4::identity());
    Program::updateUniforms();
    globs->pmesh.drawPatches();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    globs->text1.draw();
    globs->text2.draw();
    globs->text3.draw();
    globs->text4.draw();
    globs->text5.draw();
}


