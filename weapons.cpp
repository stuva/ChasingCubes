/*
 * weapons.cpp
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
#include <vector>
#include "define.h"
#include "weapons.h"

Trap::Trap( int &last_x, int &last_y ) {
	x = last_x;
	y = last_y;
	birth = SDL_GetTicks();
}

void Trap::show() {
	glPushMatrix();
	glTranslatef( x, y, 0 );
	glBegin( GL_QUADS );
		//front
		glColor4f( 1, 0, 0, 1 );
		glVertex3f( 0, 0, 0 );
		glVertex3f( L, 0, 0 );
		glVertex3f( L, L, 0 );
		glVertex3f( 0, L, 0 );
	glEnd();
	glPopMatrix();
}

void trap_show( std::vector <Trap*> &trap ) {
	for ( int i = 0; i < trap.size(); i++ ) {
		trap[i]->show();
	}
}
