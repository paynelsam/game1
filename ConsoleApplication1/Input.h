#ifndef HEADER_INPUT
#define HEADER_INPUT

class Keyboard {
public:
	bool key_right_down;
	bool key_left_down;
	bool key_up_down;
	Keyboard() : key_right_down(false), key_left_down(false) {}
	void handle_input(SDL_Event event);
};

#endif