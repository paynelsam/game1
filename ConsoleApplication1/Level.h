#include "stdafx.h"
#include <ctime>
#include "SDL.h"
#include "SDL_opengl.h"
#include "Common.h"
#include "Input.h"
#include <vector>

#ifndef HEADER_LEVEL
#define HEADER_LEVEL



class Point2D {
public:
	float x;
	float y;
	Point2D (float ix, float iy) {
		x = ix; 
		y = iy;
	}
};

class Collision2D: public Point2D {
public: 
	bool collision;
	Collision2D(bool c, float ix, float iy) : Point2D(ix, iy) {
		collision = c; 
	}
};

class Character {
public:
	float prev_x;
	float prev_y;
	float position_x; //position in meters
	float position_y; //position in meters
	float y_velocity; //speed in meters/s
	float width; //width in meters
	float height;//width in meters
	float move_velocity; //meters/second
	float x_velocity;
	bool jumping;
	bool double_jumping;
	bool wall_sliding_right;
	bool wall_sliding_left;
	Character(void) : 
		x_velocity(0), 
		y_velocity(0), 
		jumping(false), 
		double_jumping(false),
		wall_sliding_left(false),
		wall_sliding_right(false)
	{}
	void render(SDL_Renderer* renderer, float camera_position);
	void update(float dt);
};

class Player: public Character {
public:
	float hit_points;
	bool invulnerable;
	bool walking;
	Player_Keybinding keybinding;

	Player(float mv=12.5) : hit_points(100) {
		move_velocity = mv;
		width = PLAYER_WIDTH;
		height = PLAYER_HEIGHT;
	}
	void update(float dt);
};

class Platform {
public: 
	float height;
	float width;
	float position_x;
	float position_y;
	Platform(float x, float y, float w, float h) {
		position_x = x;
		position_y = y;
		width = w;
		height = h;
	}
	void render(SDL_Renderer* renderer, float camera_position);
	Collision2D check_collision(Character* player);
	Collision2D find_point_intersect(float x, float y);
	Collision2D find_line_intersect (float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
};

class Level {
public:
	std::vector<Player*> players;
	std::vector<Character*> characters;
	std::vector<Platform*> platforms;
	float gravity;
	float camera_position;

	Level() : gravity((float)100){
		Player* player1 = new Player();
		Player* player2 = new Player();
		players.push_back(player1);
		players.push_back(player2);

		/* set initial positions for players */
		player1->position_x = (float)METERS_PER_WINDOW_WIDTH/2;
		player1->position_y = (float)METERS_PER_WINDOW_HEIGHT/2;
		player2->position_x = (float)METERS_PER_WINDOW_WIDTH/2;
		player2->position_y = (float)METERS_PER_WINDOW_HEIGHT/2;
		/* set player keybindings */
		player1->keybinding.move_left = SDLK_LEFT;
		player1->keybinding.move_right = SDLK_RIGHT;
		player1->keybinding.jump = SDLK_SPACE;
		player1->keybinding.move_left_2 = SDLK_LEFT;
		player1->keybinding.move_right_2 = SDLK_RIGHT;
		player1->keybinding.jump_2 = SDLK_SPACE;
		player1->keybinding.keyboard_id = 0;
		player1->keybinding.controller_id = 0;



		/* setup platforms in level */
		Platform* platform1 = new Platform(10, METERS_PER_WINDOW_HEIGHT - PLATFORM_THICKNESS, 500, PLATFORM_THICKNESS);
		Platform* platform2 = new Platform(10, 10, PLATFORM_THICKNESS, 100) ;
		Platform* platform3 = new Platform(30, METERS_PER_WINDOW_HEIGHT - PLATFORM_THICKNESS * 4, 10, PLATFORM_THICKNESS);
		platforms.push_back(platform1);
		platforms.push_back(platform2);
		platforms.push_back(platform3);
	}
	~Level(void) {
		for (std::vector<Player*>::iterator player = players.begin(); player != players.end(); player++) {
			free((*player));
		}
		for (std::vector<Character*>::iterator character = characters.begin(); character != characters.end(); character++) {
			free((*character));
		}
		for (std::vector<Platform*>::iterator platform = platforms.begin(); platform != platforms.end(); platform++) {
			free((*platform));
		}
	}


	int update(float dt);
	void render(SDL_Renderer* renderer);
};

#endif