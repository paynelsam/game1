#include "stdafx.h"
#include "Common.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "Level.h"
#include "Input.h"

#define SMALL_DELTA (float)0.00001

void render_rectangle(SDL_Renderer* renderer, float x, float y, float width, float height, unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
	glBegin(GL_QUADS); //GL_POINT, GL_LINE, GL_LINE_STRIP, GL_LINE_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLLYGON
	{
		/* scale from meters to pixels */
		SDL_SetRenderDrawColor(renderer, R, G, B, A);
		SDL_Rect rect = {
			(int)(x*PIXELS_PER_METER), 
			(int)(y *PIXELS_PER_METER), 
			(int)(width *PIXELS_PER_METER), 
			(int)(height * PIXELS_PER_METER) };
		SDL_RenderFillRect(renderer, &rect);
		
	}	
	glEnd(); //done defining shape
}

void Character::render(SDL_Renderer* renderer){
	render_rectangle(renderer, position_x, position_y, width, height, 255, 150, 150, 255);
};

void Character::update(float dt) {
	prev_x = position_x;
	prev_y = position_y;
	position_x += x_velocity * dt;
	position_y += y_velocity * dt;
}

void Player::update(Keyboard keyboard, float dt) {
	/* update player input for movement */
	if(keyboard.key_left_down && !keyboard.key_right_down) 
		x_velocity = -move_velocity;
	else if(keyboard.key_right_down && !keyboard.key_left_down)
		x_velocity = move_velocity;
	else
		x_velocity = 0;

	if(keyboard.event_space_down) {
		if (double_jumping)
			goto jumping_done;
		else if (jumping) {
			double_jumping = true;
		} else {
			jumping = true;
		}

		y_velocity = -30;
		/*
		if (wall_sliding_left)
		x_velocity = move_velocity*3;
		if (wall_sliding_right)
		x_velocity = -move_velocity*3;
		*/
	}
jumping_done:
	Character::update(dt);
	return;
}

void Platform::render(SDL_Renderer* renderer) {
	render_rectangle(renderer, position_x, position_y, width, height, 150, 150, 150, 255);
}
Collision2D Platform::check_collision(Character* character) {
	float bx = position_x;
	float by = position_y;
	float bw = width;
	float bh = height;
	float px = character->position_x;
	float py = character->position_y;
	float pw = character->width;
	float ph = character->height;
	Collision2D ret(false, 0, 0);

	/*if (!(bx < px + pw && bx + bw > px && by < py + ph && by + bh > py)) {
	character->wall_sliding_right = false;
	character->wall_sliding_left = false;
	return ret;
	}*

	/* check top line with bottom points */
	ret = find_line_intersect(px, py + ph, character->prev_x, character->prev_y + ph, bx, by, bx+bw, by);
	if (ret.collision) {
		character->position_y = by-ph-SMALL_DELTA;
		character->y_velocity = 0;
		character->wall_sliding_right = false;
		character->wall_sliding_left = false;
		return ret;
	}

	ret = find_line_intersect(px + pw, py + ph, character->prev_x + pw, character->prev_y + ph, bx, by, bx+bw, by);
	if (ret.collision) {
		character->position_y = by-ph- SMALL_DELTA;
		character->y_velocity = 0;
		character->wall_sliding_right = false;
		character->wall_sliding_left = false;
		return ret;
	}
	/* check bottom line with top points */
	ret = find_line_intersect(px, py, character->prev_x, character->prev_y, bx, by + bh, bx+bw, by + bh);
	if (ret.collision) {
		character->position_y = by+bh + SMALL_DELTA;
		character->y_velocity = 0;
		character->wall_sliding_right = false;
		character->wall_sliding_left = false;
		return ret;
	}
	ret = find_line_intersect(px+pw, py, character->prev_x + pw, character->prev_y, bx, by + bh, bx+bw, by + bh);
	if (ret.collision) {
		character->position_y = by+bh + SMALL_DELTA;
		character->y_velocity = 0;
		character->wall_sliding_right = false;
		character->wall_sliding_left = false;
		return ret;
	}
	/* check left line with right points */
	ret = find_line_intersect(px + pw, py, character->prev_x + pw, character->prev_y, bx, by, bx, by + bh);
	if (ret.collision) {
		character->position_x = bx - pw - SMALL_DELTA;
		character->wall_sliding_right = true;
		return ret;
	}
	ret = find_line_intersect(px + pw, py + ph, character->prev_x + pw, character->prev_y + ph, bx, by, bx, by + bh);
	if (ret.collision) {
		character->position_x = bx - pw - SMALL_DELTA;
		character->wall_sliding_right = true;
		return ret;
	}
	/* check right line with left points */
	ret = find_line_intersect(px, py, character->prev_x, character->prev_y, bx + bw, by, bx + bw, by + bh);
	if (ret.collision) {
		character->position_x = bx + bw + SMALL_DELTA;
		character->wall_sliding_left = true;
		return ret;
	}
	ret = find_line_intersect(px, py + ph, character->prev_x, character->prev_y + ph, bx + bw, by, bx + bw, by + bh);
	if (ret.collision) {
		character->position_x = bx + bw + SMALL_DELTA;
		character->wall_sliding_left = true;
		return ret;
	}
	return ret;
}

Collision2D Platform::find_line_intersect (float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
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
	if (y4 == y3) {
		target_y = y4;
		if ((y1 >= y2 && !(target_y <= y1 && target_y >= y2)) ||
			(y1 <= y2 && !(target_y >= y1 && target_y <= y2)))
			return ret;

		ret.x = x1 + m1 * (target_y - y2);
		if (x3 <= x4 && ret.x >= x3 && ret.x <= x4)
			ret.collision = true;
		if (x3 >= x4 && ret.x <= x3 && ret.x >= x4)
			ret.collision = true;
	}	
	else if(x4 == x3) {
		target_x = x4;
		if ((x1 <= x2 && !(target_x >= x1 && target_x <= x2)) ||
			(x1 >= x2 && !(target_x <= x1 && target_x >= x2)))
			return ret;

		ret.y = y1 + m1 * (target_x - x2);
		if (y3 <= y4 && ret.y >= y3 && ret.y <= y4)
			ret.collision = true;
		if (y3 >= y4 && ret.y <= y3 && ret.y >= y4)
			ret.collision = true;
	}

	return ret;
}

void Level::update(Keyboard keyboard, float dt) {
	/* perform global updates */
	for (std::vector<Player>::iterator player = players.begin(); player != players.end(); player++) {
		player->y_velocity += gravity * dt;
		/* perform non-static updates */
		player->update(keyboard, dt);
	}

	/* check for static to non-static collisions */
	for (std::vector<Platform>::iterator platform = platforms.begin(); platform != platforms.end(); platform++) {
		for (std::vector<Player>::iterator player = players.begin(); player != players.end(); player++) {
			Collision2D c = platform->check_collision(&(*player));
			if(c.collision) {
				player->double_jumping = false;
				player->jumping = false;
				player->y_velocity = player->y_velocity < 5 ? player->y_velocity : 5;
			}
		}
	}

	/* check for sprite to non-static collisions */
}

void Level::render(SDL_Renderer* renderer) {
	for (std::vector<Player>::iterator player = players.begin(); player != players.end(); player++) {
		player->render(renderer);
	}
	for (std::vector<Platform>::iterator platform = platforms.begin(); platform != platforms.end(); platform++) {
		platform->render(renderer);
	}
}