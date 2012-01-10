/*
 * pg.cpp
 * Copyright (C) Michele Cucca 2012 <miche.cucca@gmail.com>
 * 
 * ChasingCubes is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ChasingCubes is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "vector"
#include "define.h"
#include "world.h"
#include "weapons.h"
#include "pg.h"
#include "fruit.h"

Fruit::Fruit(){
	angle = 0;
	x = ( rand() % 40 ) - 20;
	y = ( rand() % 30 ) - 15;
	x *= L;
	y *= L;
}

void Fruit::check( int &last_x, int &last_y, unsigned int &score, int nenemies, Pg &pg ) {
	if ( last_x == x  && last_y == y ) {
		angle = 0;
		x = ( rand() % 40 ) - 20;
		y = ( rand() % 30 ) - 15;
		x *= L;
		y *= L;
		score += nenemies;
		pg.ntrap += 1 + rand() % 2;
	}
}

void Fruit::show() {
	glPushMatrix();
	glTranslatef( x+L/2, y+L/2, 0 );
	glRotatef( angle, 0, 0, 1 );
	angle += 3;
	glBegin( GL_QUADS );
		//front
		glColor4f( 0.3, 0.7, 0.8, 1 );
		glVertex3f( -L/2, -L/2, 0 );
		glVertex3f(  L/2, -L/2, 0 );
		glVertex3f(  L/2,  L/2, 0 );
		glVertex3f( -L/2,  L/2, 0 );
	glEnd();
	glPopMatrix();

}

