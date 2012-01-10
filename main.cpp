/*
 * main.cpp
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
#include <iostream>
#include <sstream>
#include <fstream>

#include "define.h"
#include "world.h"
#include "weapons.h"
#include "pg.h"
#include "enemy.h"
#include "fruit.h"

void init() {
	SDL_Init( SDL_INIT_EVERYTHING );
	SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL );
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	//glFrustum( 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 100, 1000);
	// scritto in questo modo l'origine è al centro dello schermo
	glFrustum( -SCREEN_WIDTH/2, SCREEN_WIDTH/2, -SCREEN_HEIGHT/2, SCREEN_HEIGHT/2, 900, 1500);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
		glRotatef( 20, -1, 0, 0 );
		glTranslatef( 0, 400, -1000 );
	SDL_WM_SetCaption( "ChasingCubes", NULL );
	glShadeModel( GL_SMOOTH );
	glEnable( GL_DEPTH_TEST );
	//glDepthFunc( GL_LEQUAL );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}


// TODO: sistema questa cosa dei livelli
int level( unsigned int &score ){
	return score/200 + 1;
}

int main( int argc, char *argv[] ) {
	if(argc == 2 && !strcmp("-v", argv[1])) {
		std::cout << "ChasingCubes 20120110-alpha, Copyright (C) 2012 Michele Cucca, see LICENSE for details\n";
		return 0;
	}

	srand48( time( NULL ) );

	bool quit = false;

	SDL_Event event;

	// ultima posizione del pg
	int last_x, last_y;

	// true when you lose
	bool you_lose = false;

	// punteggio
	unsigned int score = 0;

	PosMatrix posmatrix;
	Pg pg;
	std::vector <Trap*> trap;
	std::vector <Enemy*> enemy;
	Fruit fruit;

	init();

	// main loop
	while ( ( quit == false ) && ( ! you_lose ) ) {
		create_enemy( posmatrix, enemy );

		/////////////// EVENTI
		while( SDL_PollEvent( &event ) ) {
			if ( event.type == SDL_QUIT ) {
				quit = true;
			}
		}
		pg.handle_input( event, posmatrix, you_lose, last_x, last_y, trap );

		/////////////// LOGICA
		enemy_follow( posmatrix, enemy, last_x, last_y  );
		fruit.check( last_x, last_y, score, enemy.size(), pg );

		/////////////// MOVIMENTO 
		pg.move( posmatrix );
		enemy_move( posmatrix, enemy );
		trap_check( posmatrix, trap, enemy );

		/////////////// RENDERING
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glPushMatrix();
			float phi = -(float)last_x/100;
			float theta = (float)last_y/100;
			glRotatef( phi, 0, 0, 5 );
			glRotatef( theta, 1, 0, 0 );
			ground();
			pg.show( last_x, last_y );
			enemy_show( enemy );
			trap_show( trap );
			fruit.show();
//			posmatrix.show(); // mostra le posizioni occupate (diagnostica)
		glPopMatrix();
		SDL_GL_SwapBuffers();

		// fissa il framerate
//		SDL_Delay( 1000 / FRAMES_PER_SECOND );
	}

	// print score
	std::ofstream score_file("score");
	score_file << score << "\n";
	score_file.close();
	std::cout << score;

	// quit
	SDL_Quit();
}
