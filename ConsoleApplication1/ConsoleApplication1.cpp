// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ctime>
#include "SDL.h"
#include "SDL_opengl.h"
#include "Level.h"
#include "Common.h"
#include "Input.h"

Keyboard keyboard1;


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
		keyboard1.event_space_down = false;
		while (SDL_PollEvent(&event)) {
			/* if exit, exit */
			if(event.type == SDL_QUIT)
				isRunning = false;
			if(event.type == SDL_KEYUP || event.type == SDL_KEYDOWN) {
				keyboard1.handle_input(event);
			}
		}
		/********************************************************
		* game state updates - all units in meters
		********************************************************/
		level1.update(keyboard1, delta_t);

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

