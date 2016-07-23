#include "stdafx.h"
#include <ctime>
#include "SDL.h"
#include "SDL_opengl.h"
#include "Level.h"
#include "Common.h"
#include "Input.h"

void Keyboard::handle_input(SDL_Event event) {
	if(event.type == SDL_KEYUP) {
		switch (event.key.keysym.sym) {
		case SDLK_ESCAPE: 
			break;
		case SDLK_r:
			glClearColor(1,0,0,1); //white
			break;
		case SDLK_RIGHT:
			key_right_down = false;
			break; 
		case SDLK_LEFT:
			key_left_down = false;
			break; 
		case SDLK_UP:
			key_up_down = false;
			break;
		}
	}
	if(event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
		case SDLK_RIGHT:
			key_right_down = true;
			break; 
		case SDLK_LEFT:
			key_left_down = true;
			break; 
		case SDLK_UP:
			key_up_down = true;
			break;
		}
	}
}