

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
            if( k == SDLK_F1 || k == SDLK_F2 ){
                globs->subdivs += (k==SDLK_F1 ? 1:-1 ) * 0.25;
                std::ostringstream oss;
                oss << "Tess level: " << globs->subdivs;
                globs->text5.setText(oss.str());
            }
                
            if( k == SDLK_F3 ){
                globs->wireframe = !globs->wireframe;
                if( globs->wireframe )
                    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                else
                    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
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
    
    if( globs->keyset.find(SDLK_w) != globs->keyset.end() )
        globs->camera.walk(walkAmt*elapsed);
    if( globs->keyset.find(SDLK_s) != globs->keyset.end() )
        globs->camera.walk(-walkAmt*elapsed);
    if( globs->keyset.find(SDLK_a) != globs->keyset.end() )
        globs->camera.turn(turnAmt*elapsed);
    if( globs->keyset.find(SDLK_d) != globs->keyset.end() )
        globs->camera.turn(-turnAmt*elapsed);
    if( globs->keyset.find(SDLK_r) != globs->keyset.end() )
        globs->camera.tilt(turnAmt*elapsed);
    if( globs->keyset.find(SDLK_f) != globs->keyset.end() )
        globs->camera.tilt(-turnAmt*elapsed);
    if( globs->keyset.find(SDLK_i) != globs->keyset.end() )
        globs->camera.strafe(0,walkAmt*elapsed,0);
    if( globs->keyset.find(SDLK_k) != globs->keyset.end() )
        globs->camera.strafe(0,-walkAmt*elapsed,0);
    if( globs->keyset.find(SDLK_j) != globs->keyset.end() )
        globs->camera.strafe(-walkAmt*elapsed,0,0);
    if( globs->keyset.find(SDLK_l) != globs->keyset.end() )
        globs->camera.strafe(walkAmt*elapsed,0,0);
        
}

  
