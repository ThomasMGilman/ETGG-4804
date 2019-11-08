template<typename T>
bool keyInKeyset(T key)
{
	return globs->keyset.find(key) != globs->keyset.end();
}

void update(int elapsed){
    SDL_Event ev;
    while(true){
        if(!SDL_PollEvent(&ev))
            break;
        if(ev.type == SDL_QUIT){
            SDL_Quit();
            exit(0);
        }
        else if( ev.type == SDL_KEYDOWN ){
            auto k = ev.key.keysym.sym;
            globs->keyset.insert(k);
            
            if( k == SDLK_p )
                globs->paused = !globs->paused;
            if(k == SDLK_q){
                SDL_Quit();
                exit(0);
            }
			if (k == SDLK_F1 || k == SDLK_F2) {
				globs->subdivs += (k == SDLK_F1 ? 1 : -1) * 0.25;
				std::ostringstream oss;
				oss << "Tess level: " << globs->subdivs;
				globs->text5.setText(oss.str());
			}
			if (k == SDLK_F3) {
				globs->wireframe = !globs->wireframe;
				if (globs->wireframe)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
        }
        else if( ev.type == SDL_KEYUP ){
            auto k = ev.key.keysym.sym;
            globs->keyset.erase(k);
        }
        else if( ev.type == SDL_MOUSEBUTTONDOWN ){
            //ev.button.button,ev.button.x,ev.button.y)
        }
        else if( ev.type == SDL_MOUSEBUTTONUP ){
            //print("mouse up:",ev.button.button,ev.button.x,ev.button.y)
        }
        else if(ev.type == SDL_MOUSEMOTION ){
            //print("mouse move:",ev.motion.x,ev.motion.y)
        }
    }

    if(globs->paused)
        return;

    const float turnAmt = 0.0007f;
    const float walkAmt = 0.005f;
    
	//Camera Movement
	if (keyInKeyset(SDLK_w))
		globs->scene.walkCamera(walkAmt * elapsed);
	if (keyInKeyset(SDLK_s))
		globs->scene.walkCamera(-walkAmt * elapsed);

	//Camera Strafing
    if (keyInKeyset(SDLK_a))
		globs->scene.strafeCamera(-walkAmt * elapsed, 0, 0);
    if (keyInKeyset(SDLK_d))
		globs->scene.strafeCamera(walkAmt * elapsed, 0, 0);
	if (keyInKeyset(SDLK_SPACE))
		globs->scene.strafeCamera(0, walkAmt * elapsed, 0);
	if (keyInKeyset(SDLK_LCTRL))
		globs->scene.strafeCamera(0, -walkAmt * elapsed, 0);
        

	//Camera Rotation
	if (keyInKeyset(SDLK_j) || keyInKeyset(SDLK_LEFT))
		globs->scene.camera.turn(turnAmt * elapsed);
	if (keyInKeyset(SDLK_l) || keyInKeyset(SDLK_RIGHT))
		globs->scene.camera.turn(-turnAmt * elapsed);

	//Camera Tilt
	if (keyInKeyset(SDLK_e))
		globs->scene.tiltCamera(turnAmt * elapsed);
	if (keyInKeyset(SDLK_r))
		globs->scene.tiltCamera(-turnAmt * elapsed);

}

  
