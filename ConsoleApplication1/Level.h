#include "stdafx.h"
#include <ctime>
#include "SDL.h"
#include "SDL_opengl.h"
#include "Common.h"
#include "Input.h"

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
		void render(void);
		void update(float dt);
};

class Player: public Character {
	public:
		float hit_points;
		bool invulnerable;
		bool walking;

		Player(float mv=12.5) : hit_points(100) {
			move_velocity = mv;
			width = 1;
			height = 3;
		}
		void update(Keyboard keyboard, float dt);
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
	void render(void);
	Collision2D check_collision(Character* player);
	Collision2D find_line_intersect (float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
};

class Level {
public:
	Player player1;
	Player player2;
	Platform platform1;
	float gravity;
	Level() : gravity((float)100), platform1(10, METERS_PER_WINDOW_HEIGHT - 10, 50, 10) {
		player1.position_x = (float)METERS_PER_WINDOW_WIDTH/2;
		player1.position_y = (float)METERS_PER_WINDOW_HEIGHT/2;
	}

	void update(Keyboard keyboard, float dt);
};

#endif