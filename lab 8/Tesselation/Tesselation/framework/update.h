template<typename T>
bool keyInKeyset(T key)
{
	return globs->keyset.find(key) != globs->keyset.end();
}

void deInitVars()
{
	globs->rayBufferA.~shared_ptr();	//FreeGLBufferMemory
	globs->rayBufferB.~shared_ptr();	//FreeGLBufferMemory
}

void update(int elapsed){
    SDL_Event ev;
    while(true){
        if(!SDL_PollEvent(&ev))
            break;
        if(ev.type == SDL_QUIT){
			deInitVars();
            SDL_Quit();
            exit(0);
        }
        else if( ev.type == SDL_KEYDOWN ){
            auto k = ev.key.keysym.sym;
            globs->keyset.insert(k);
            
            if( k == SDLK_p )
                globs->paused = !globs->paused;
            if(k == SDLK_q){
				deInitVars();
                SDL_Quit();
                exit(0);
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

    const float turnAmt = 0.005f;
    const float walkAmt = 0.01f;
    
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
        
	
	//Camera Tilt
	if (keyInKeyset(SDLK_i) || keyInKeyset(SDLK_UP))
		globs->scene.tiltCamera(turnAmt * elapsed);
	if (keyInKeyset(SDLK_k) || keyInKeyset(SDLK_DOWN))
		globs->scene.tiltCamera(-turnAmt * elapsed);
	//Camera Rotation
	if (keyInKeyset(SDLK_j) || keyInKeyset(SDLK_LEFT))
		globs->scene.camera.turn(turnAmt * elapsed);
	if (keyInKeyset(SDLK_l) || keyInKeyset(SDLK_RIGHT))
		globs->scene.camera.turn(-turnAmt * elapsed);

	//Reflection Controls
	if (keyInKeyset(SDLK_r))
	{
		globs->allowReflections = !globs->allowReflections;
		SDL_Delay(100);
	}
	if (keyInKeyset(SDLK_1))	//Decrease Reflection
	{
		if (!globs->allowReflections)
			globs->allowReflections = true;
		if (!(globs->reflectionPasses <= globs->minReflectionPasses))
			globs->reflectionPasses--;
		else
			globs->allowReflections = false;
		SDL_Delay(100);
	}
	if (keyInKeyset(SDLK_2))	//Increase Reflection
	{
		if (!(globs->reflectionPasses >= globs->maxReflectionPasses))
			globs->reflectionPasses++;
		if (!globs->allowReflections)
			globs->allowReflections = true;
		SDL_Delay(100);
	}
}

  
