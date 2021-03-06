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
#include <iostream>
#include <vector>
#include "define.h"
#include "world.h"
#include "weapons.h"
#include "pg.h"

Pg::Pg() {
	x = y = 0;
	speed = 0.5;
	moving = 'n';
	angle = 0;
	axis[0] = axis[1] = axis[2] = 0;
	ntrap = 0;
}

void Pg::handle_input( SDL_Event &event, PosMatrix &posmatrix, bool &you_lose, int &last_x, int &last_y, std::vector <Trap*> &trap ) {
	Uint8 *keystates = SDL_GetKeyState( NULL );
	if ( moving == 'n' ) { 
		if ( ( keystates[ SDLK_UP ] ) || (  keystates[SDLK_w] ) ) {
			if ( y < 280 ) {
				if ( posmatrix.is_free( x, y+L ) ) {
					posmatrix.set_busy( x, y+L );
					moving = 'u'; 
					angle = 90;
					axis[0] = 1;
					axis[1] = 0;
					axis[2] = 0;
				} else you_lose = true;
			}
		} else if ( ( keystates[ SDLK_DOWN ] ) || ( keystates[SDLK_s] ) ) {
			if ( y > -300 ) {
				if ( posmatrix.is_free( x, y-L ) ) {
					posmatrix.set_busy( x, y-L );
					moving = 'd'; 
					axis[0] = 1;
					axis[1] = 0;
					axis[2] = 0;
				} else you_lose = true;
			}
		} else if ( ( keystates[ SDLK_LEFT ] ) || ( keystates[ SDLK_a ] ) ) {
			if ( x > -400 ) {
				if ( posmatrix.is_free( x-L, y ) ) {
					posmatrix.set_busy( x-L, y );
					moving = 'l'; 
					axis[0] = 0;
					axis[1] = -1;
					axis[2] = 0;
				} else you_lose = true;
			}
		} else if ( ( keystates[ SDLK_RIGHT ] ) || ( keystates[ SDLK_d ] ) ) {
			if ( x < 380 ) {
				if ( posmatrix.is_free( x+L, y ) ) {
					posmatrix.set_busy( x+L, y );
					moving = 'r'; 
					angle = 90;
					axis[0] = 0;
					axis[1] = -1;
					axis[2] = 0;
				} else you_lose = true;
			}
		}
	}
	if ( event.type == SDL_KEYDOWN  && ( SDL_GetTicks() - last_trap ) > 100 ) {
		if ( event.key.keysym.sym == SDLK_SPACE && ntrap > 0 ) {
			ntrap--;
			last_trap = SDL_GetTicks();
			trap.push_back( new Trap( last_x, last_y ) );
		}
	}
}

void Pg::move( PosMatrix &posmatrix ) {
	Uint32 now =  SDL_GetTicks() ;
	float dt = now - t;
	float step = speed * dt;
	if ( angle > 90 ) angle = 90;
	if ( angle < 0 ) angle = 0;
	t = now;
	switch ( moving ) {
		case 'u': 
			if ( angle == 90 ) {
				// il cubo inizia a muoversi
				y += L;
			}
			if ( angle > 0 ) {
				// il cubo si sta muovendo
				angle -= step;
			} else if ( angle == 0 ) {
				// il cubo si ferma
				posmatrix.set_free( x, y-L );
				moving = 'n';
				axis[0] = axis[1] = axis[2] = 0;
			}
			break;
		case 'd': 
			if ( angle < 90 ) {
				angle += step;
			} else if ( angle == 90 ) {
				posmatrix.set_free( x, y );
				angle = 0;
				moving = 'n';
				axis[0] = axis[1] = axis[2] = 0;
				y -= L;
			}
			break;
		case 'l': 
			if ( angle < 90 ) {
				angle += step;
			} else if ( angle == 90 ) {
				posmatrix.set_free( x, y );
				angle = 0;
				moving = 'n';
				axis[0] = axis[1] = axis[2] = 0;
				x -= L;
			}
			break;
		case 'r': 
			if ( angle == 90 ) {
				x += L;
			}
			if ( angle > 0 ) {
				angle -= step;
			} else if ( angle == 0 ) {
				posmatrix.set_free( x-L, y );
				moving = 'n';
				axis[0] = axis[1] = axis[2] = 0;
			}
			break;
		case 'n': 
			posmatrix.set_busy( x, y );
			break;
	}
}

void Pg::show( int &last_x, int &last_y ) {
	last_x = x;
	last_y = y;
	glPushMatrix();
	glTranslatef( x, y, 0 );
	glRotatef( angle, axis[0], axis[1], axis[2] );
	glBegin( GL_QUADS );
		//front
		glColor4f( 1, 1, 1, 1 );
		glVertex3f( 0, 0, L );
		glVertex3f( L, 0, L );
		glVertex3f( L, L, L );
		glVertex3f( 0, L, L );
		//rear
		glColor4f( 0.5, 0.5, 0.5, 1 );
		glVertex3f( 0, 0, 0 );
		glVertex3f( L, 0, 0 );
		glVertex3f( L, L, 0 );
		glVertex3f( 0, L, 0 );
		//left
		glColor4f( 0, 0, 1, 1 );
		glVertex3f( 0, 0, L );
		glVertex3f( 0, L, L );
		glVertex3f( 0, L, 0 );
		glVertex3f( 0, 0, 0 );
		//right
		glColor4f( 0, 1, 1, 1 );
		glVertex3f( L, L, L );
		glVertex3f( L, 0, L );
		glVertex3f( L, 0, 0 );
		glVertex3f( L, L, 0 );
		//top
		glColor4f( 1, 1, 0, 1 );
		glVertex3f( 0, L, L );
		glVertex3f( L, L, L );
		glVertex3f( L, L, 0 );
		glVertex3f( 0, L, 0 );
		//bottom
		glColor4f( 1, 0, 1, 1 );
		glVertex3f( 0, 0, L );
		glVertex3f( 0, 0, 0 );
		glVertex3f( L, 0, 0 );
		glVertex3f( L, 0, L );
	glEnd();
	glPopMatrix();
}


