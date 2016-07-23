#ifndef HEADER_INPUT
#define HEADER_INPUT

class Player_Keybinding {
public:
	SDL_Keycode move_left;
	SDL_Keycode move_right;
	SDL_Keycode jump;
	SDL_Keycode shoot;
	SDL_Keycode move_left_2;
	SDL_Keycode move_right_2;
	SDL_Keycode jump_2;
	SDL_Keycode shoot_2;

	bool move_left_down;
	bool move_right_down;
	bool jump_down;
	bool shoot_down;
	bool jump_down_event;
	int keyboard_id;
	int controller_id;
	void handle_input(SDL_Event event);

	Player_Keybinding() {
		move_left_down = false;
		move_right_down = false;
		jump_down = false;
		shoot_down = false;
		jump_down_event = false;
	}
};

#endif