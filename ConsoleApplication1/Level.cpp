#include "stdafx.h"
#include "Common.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "Level.h"

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

void Character::render(void){
	render_rectangle(position_x, position_y, width, height, 255, 150, 150, 255);
};

void Character::update(float dt) {
	prev_x = position_x;
	prev_y = position_y;
	position_x += x_velocity * dt;
	position_y += y_velocity * dt;
}

void Platform::render(void) {
	render_rectangle(position_x, position_y, width, height, 150, 150, 150, 255);
}
Collision2D Platform::check_collision(Character* player) {
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

void Level::update(float dt) {
	/* perform global updates */
	player1.y_velocity += gravity * dt;

	/* perform non-static updates */
	player1.update(dt);

	/* check for static to non-static collisions */
	Collision2D c = platform1.check_collision(&player1);

	/* check for sprite to non-static collisions */
}