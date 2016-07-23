// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ctime>
#include "SDL.h"
#include "SDL_opengl.h"
#include "Level.h"
#include "Common.h"
#include "Input.h"
#include <SDL_gamecontroller.h>

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
	//SDL_WM_SetCaption("My First Game", NULL);

	/* set window size and color depth */
	//SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_OPENGL);

	SDL_Window *screen = SDL_CreateWindow("My First Game",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(screen, -1, 0);


	// Up until now everything was drawn behind the scenes.
	// This will show the new, red contents of the window.
	SDL_RenderPresent(renderer);
	/* color is used when the screen is cleared */

	glClearColor(1,1,1,1); //white
	/* set the par tof the screen to be displayed */
	glViewport(0,0,WINDOW_WIDTH, WINDOW_HEIGHT);
	glShadeModel(GL_SMOOTH);

	/* configure 2D GL parameters */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //save gl config
	glDisable(GL_DEPTH_TEST);

	/* open all controllers */
	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		if (SDL_IsGameController(i))
			SDL_GameControllerOpen(i);
	}

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

		/********************************************************
		* render to screen - all units in pixels
		********************************************************/
		/* clear screen */
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

		// Clear the entire screen to our selected color.
		SDL_RenderClear(renderer);

		level1.render(renderer);

		SDL_RenderPresent(renderer);
		
		/* adjust matrix to window dimensions */
		//glOrtho(0, METERS_PER_WINDOW_WIDTH, METERS_PER_WINDOW_HEIGHT, 0, -1, 1);

		SDL_GL_SwapWindow(screen);
	}

	//SDL_Delay(5000); //Milliseconds
	SDL_Quit();
	return 0;
}

