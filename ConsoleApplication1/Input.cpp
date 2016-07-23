#include "stdafx.h"
#include <ctime>
#include "SDL.h"
#include "SDL_opengl.h"
#include "Level.h"
#include "Common.h"
#include "Input.h"


void Player_Keybinding::handle_input (SDL_Event event) {
	/* process event */	
	if(event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == move_left) 
			move_left_down = true;
		if (event.key.keysym.sym == move_right)
			move_right_down = true;
		if (event.key.keysym.sym == jump) {
			jump_down = true;
			jump_down_event = true;
		}
	}
	if (event.type == SDL_KEYUP) {
		if (event.key.keysym.sym == move_left)
			move_left_down = false;
		if (event.key.keysym.sym == move_right)
			move_right_down = false;
		if (event.key.keysym.sym == jump)
			jump_down = false;
	}
	if (event.type == SDL_CONTROLLERBUTTONDOWN && event.cbutton.which == controller_id) {
		if (event.cbutton.button == move_left)
			move_left_down = true;
		if (event.cbutton.button == move_right)
			move_right_down = true;
		if (event.cbutton.button == jump) {
			jump_down = true;
			jump_down_event = true;
		}
	}
	if (event.type ==  SDL_CONTROLLERBUTTONUP && event.cbutton.which == controller_id) {
		if (event.cbutton.button == move_left)
			move_left_down = false;
		if (event.cbutton.button == move_right)
			move_right_down = false;
		if (event.cbutton.button == jump)
			jump_down = false;
	}
	if (event.type == SDL_CONTROLLERAXISMOTION && event.caxis.which == controller_id) {
		printf("%d %d %d %d \n", event.caxis.axis, event.caxis.type, event.caxis.value, event.caxis.which);
		if (event.caxis.type == move_left)
			move_left_down = true;
		if (event.caxis.type == move_right)
			move_right_down = true;
	}
}