// ConsoleApplication1.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"
#include <ctime>
#include "SDL.h"
#include "SDL_opengl.h"

#define WINDOW_WIDTH 1280	
#define WINDOW_HEIGHT 720
#define METERS_PER_WINDOW_HEIGHT 50.0
#define METERS_PER_WINDOW_WIDTH METERS_PER_WINDOW_HEIGHT/WINDOW_HEIGHT * WINDOW_WIDTH  //based on aspect ratio
#define PIXELS_PER_METER WINDOW_HEIGHT/METERS_PER_WINDOW_HEIGHT

void render_rectangle(float x, float y, float width, float height, unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
	glBegin(GL_QUADS); //GL_POINT, GL_LINE, GL_LINE_STRIP, GL_LINE_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLLYGON
	{
		glColor4ub(R, G, B, A); //could do 3ub and not specify alpha		
		glVertex2f(x, y); 
		glVertex2f(x, y + height); 
		glVertex2f(x + width, y + height); //could do 3ub and not specify alpha		
		glVertex2f(x + width, y); 
	}	
	glEnd(); //done defining shape
}


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
		void render(void) {
			render_rectangle(position_x, position_y, width, height, 255, 150, 150, 255);
		}
		void update(float dt) {
			prev_x = position_x;
			prev_y = position_y;
			position_x += x_velocity * dt;
			position_y += y_velocity * dt;
		}
};

class Player: public Character {
	public:
		float hit_points;
		bool invulnerable;
		bool falling;
		bool walking;
		bool jumping;
		Player(float mv=12.5) : hit_points(100) {
			move_velocity = mv;
			width = 1;
			height = 3;
		}
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
	void render(void) {
		render_rectangle(position_x, position_y, width, height, 150, 150, 150, 255);
	}
	Collision2D check_collision(Character* player) {
		float bx = position_x;
		float by = position_y;
		float bw = width;
		float bh = height;
		float px = player->position_x;
		float py = player->position_y;
		float pw = player->width;
		float ph = player->height;
		Collision2D ret(false, 0, 0);

		if (!(bx < px + pw && bx + bw > px && by < py + ph && by + bh > py)) {
			return ret;
		}

		/* check top line with bottom points */
		ret = find_line_intersect(px, py + ph, player->prev_x, player->prev_y + ph, bx, by, bx+bw, by);
		if (ret.collision) {
			player->position_y = by-ph;
			player->y_velocity = 0;
			return ret;
		}
		ret = find_line_intersect(px + pw, py + ph, player->prev_x + pw, player->prev_y + ph, bx, by, bx+bw, by);
		if (ret.collision) {
			player->position_y = by-ph;
			player->y_velocity = 0;
			return ret;
		}
		/* check bottom line with top points */
		ret = find_line_intersect(px, py, player->prev_x, player->prev_y, bx, by + bh, bx+bw, by + bh);
		if (ret.collision) {
			player->position_y = by+bh;
			player->y_velocity = 0;
			return ret;
		}
		ret = find_line_intersect(px+pw, py, player->prev_x + pw, player->prev_y, bx, by + bh, bx+bw, by + bh);
		if (ret.collision) {
			player->position_y = by+bh;
			player->y_velocity = 0;
			return ret;
		}
		/* check left line with right points */
		ret = find_line_intersect(px + pw, py, player->prev_x + pw, player->prev_y, bx, by, bx, by + bh);
		if (ret.collision) {
			player->position_x = bx - pw;
			return ret;
		}
		ret = find_line_intersect(px + pw, py + ph, player->prev_x + pw, player->prev_y + ph, bx, by, bx, by + bh);
		if (ret.collision) {
			player->position_x = bx - pw;
			return ret;
		}
		/* check right line with left points */
		ret = find_line_intersect(px, py, player->prev_x, player->prev_y, bx + bw, by, bx + bw, by + bh);
		if (ret.collision) {
			player->position_x = bx + bw;
			return ret;
		}
		ret = find_line_intersect(px, py + ph, player->prev_x, player->prev_y + ph, bx + bw, by, bx + bw, by + bh);
		if (ret.collision) {
			player->position_x = bx + bw;
			return ret;
		}
		return ret;
	}
	
	Collision2D find_line_intersect (float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
		Collision2D ret(false, 0, 0);
		if (x1 == x2) {
			if (y2 > y1 && y4 <= y2 && y4 >= y1 ||
				y1 > y2 && y4 <= y1 && y4 >= y2)
				ret.collision = true;
			return ret;	
		}

		float m1 = (y2 - y1) / (x2 - x1);
		float target_y;
		float target_x;
		//XXX: assumes we cannot intersect at 0
		if (y4 == y3) {
			target_y = y4;
			if (y1 > y2 && !(target_y <= y1 && target_y >= y2))
				return ret;
			if (y1 < y2 && !(target_y >= y1 && target_y <= y2))
				return ret;
			
			ret.x = x1 + m1 * (target_y - y2);
			if (x3 < x4 && ret.x >= x3 && ret.x <= x4)
				ret.collision = true;
			if (x3 > x4 && ret.x <= x3 && ret.x >= x4)
				ret.collision = true;
		}	
		else if(x4 == x3) {
			target_x = x4;
			if (x1 < x2 && !(target_x >= x1 && target_x <= x2))
				return ret;
			if (x1 > x2 && !(target_x <= x1 && target_x >= x2))
				return ret;
			
			ret.y = y1 + m1 * (target_x - x2);
			if (y3 < y4 && ret.y >= y3 && ret.y <= y4)
				ret.collision = true;
			if (y3 > y4 && ret.y <= y3 && ret.y >= y4)
				ret.collision = true;
		}

		return ret;
	}
};

class Keyboard {
public:
	bool key_right_down;
	bool key_left_down;
	bool key_up_down;
	Keyboard() : key_right_down(false), key_left_down(false) {}
};

Keyboard keyboard1;

class Level {
public:
	Player player1;
	Player player2;
	Platform platform1;
	float gravity;
	Level() : gravity((float)9.8), platform1(10, METERS_PER_WINDOW_HEIGHT - 10, 50, 10) {
	player1.position_x = (float)METERS_PER_WINDOW_WIDTH/2;
	player1.position_y = (float)METERS_PER_WINDOW_HEIGHT/2;
	
	}

	void update(float dt) {
		/* perform global updates */
		player1.y_velocity += gravity * dt;

		/* perform non-static updates */
		player1.update(dt);

		/* check for static to non-static collisions */
		Collision2D c = platform1.check_collision(&player1);
			
		/* check for sprite to non-static collisions */
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	Level level1;
	clock_t current_t = clock();
	clock_t previous_t;
	float delta_t; //in seconds

	/* inits everything, audio, video joystick, etc. */
	SDL_Init(SDL_INIT_EVERYTHING);

	/* setup space used for each color, and other memory usage */
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);

	/* set window caption */
	SDL_WM_SetCaption("My First Game", NULL);

	/* set window size and color depth */
	SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_OPENGL);

	/* color is used when the screen is cleared */
	glClearColor(1,1,1,1); //white
	/* set the par tof the screen to be displayed */
	glViewport(0,0,WINDOW_WIDTH, WINDOW_HEIGHT);
	glShadeModel(GL_SMOOTH);

	/* configure 2D GL parameters */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //save gl config
	glDisable(GL_DEPTH_TEST);

	/* setup game state */


	/* main loop */
	bool isRunning = true;
	SDL_Event event;
	while (isRunning) {
		previous_t = current_t;
		current_t = clock();
		delta_t = ((float)(current_t - previous_t))/CLOCKS_PER_SEC; // in seconds
		/********************************************************
		* events - input
		********************************************************/
		/* loop through all events */
		while (SDL_PollEvent(&event)) {
			/* if exit, exit */
			if(event.type == SDL_QUIT)
				isRunning = false;
			if(event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE: 
					isRunning = false;
					break;
				case SDLK_r:
					glClearColor(1,0,0,1); //white
					break;
				case SDLK_RIGHT:
					keyboard1.key_right_down = false;
					break; 
				case SDLK_LEFT:
					keyboard1.key_left_down = false;
					break; 
				case SDLK_UP:
					keyboard1.key_up_down = false;
					break;
				}
			}
			if(event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_RIGHT:
					keyboard1.key_right_down = true;
					break; 
				case SDLK_LEFT:
					keyboard1.key_left_down = true;
					break; 
				case SDLK_UP:
					keyboard1.key_up_down = true;
					break;
				}
			}
		}
		/********************************************************
		* game state updates - all units in meters
		********************************************************/
		if(keyboard1.key_left_down && !keyboard1.key_right_down) 
			level1.player1.x_velocity = -1 * level1.player1.move_velocity;
		else
		if(keyboard1.key_right_down && !keyboard1.key_left_down)
			level1.player1.x_velocity = level1.player1.move_velocity;
		else
			level1.player1.x_velocity = 0;


		level1.update(delta_t);

		/*** collision detection ***/
		/* first detect collision with platform and correct */


		/* next detect collision with projectile and subtract health */

		/********************************************************
		* render to screen - all units in pixels
		********************************************************/
		/* clear screen */
		glClear(GL_COLOR_BUFFER_BIT);

		glPushMatrix(); /* start rendering */
		/* adjust matrix to window dimensions */
		glOrtho(0, METERS_PER_WINDOW_WIDTH, METERS_PER_WINDOW_HEIGHT, 0, -1, 1);

		level1.player1.render();
		level1.platform1.render();

#if 0
		glBegin(GL_LINES); //GL_POINT, GL_LINE, GL_LINE_STRIP, GL_LINE_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLLYGON
		{
			glColor4ub(0, 0, 255, 255); //could do 3ub and not specify alpha		
			glVertex2f(0,0); //2 for 2-dimension, f for fload
			glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT); //2 for 2-dimension, f for fload		
		}	
		glEnd(); //done defining shape

		glBegin(GL_QUADS); //GL_POINT, GL_LINE, GL_LINE_STRIP, GL_LINE_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLLYGON
		{
			glColor4ub(0, 255, 0, 255); //could do 3ub and not specify alpha		
			glVertex2f(WINDOW_WIDTH*3/4,WINDOW_HEIGHT*3/4); 
			glVertex2f(WINDOW_WIDTH*0,WINDOW_HEIGHT*4/4); 
			glColor4ub(0, 255, 255, 0); //could do 3ub and not specify alpha		
			glVertex2f(WINDOW_WIDTH*1/4,WINDOW_HEIGHT*1/4); 
			glVertex2f(WINDOW_WIDTH*3/4, WINDOW_HEIGHT*1/4); 
		}	
		glEnd(); //done defining shape
#endif
		glPopMatrix(); /* end rendering */

		SDL_GL_SwapBuffers();
	}

	//SDL_Delay(5000); //Milliseconds
	SDL_Quit();
	return 0;
}

