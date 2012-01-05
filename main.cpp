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

#define PI 3.14159265

// impostazioni dello schermo
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

const int FRAMES_PER_SECOND = 60;

// dimensioni della griglia
const int GRID_WIDTH = 800;
const int GRID_HEIGHT = 600;

// lato dei quadrati
const int L = 20;

SDL_Event event;

// impostazioni delle luci
GLfloat LightAmbient[]  = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat LightDiffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 500.0f, 0.0f, 800.0f, 1.0f };

// ultima posizione del pg
int last_x, last_y;

// true when you lose
bool you_lose = false;

// punteggio
unsigned long int score = 0;

class PosMatrix {
	private:
		// la matrice rappresenta le posizioni occupabili dai cubi
		bool matrix[40][30];
		// converte le coordinate dei cubi in indici matriciali
		// TODO: usa un nome più consono
		int get_x( int x );
		int get_y( int y );
	public:
		PosMatrix();
		bool is_free( int x, int y );
		void set_free( int x, int y );
		void set_busy( int x, int y );
		// visualizza le posizioni occupate (a scopo diagnostico)
		void show();
};


PosMatrix posmatrix;

class Pg {
	private:
		float x, y;
		float angle;
		// indica la direzione
		char moving;
		// asse di rotazione
		int axis[3];
		Uint32 t;
	public:
		float speed;
		Pg();
		void handle_input();
		void move();
		void show();
};

class Enemy {
	private: 
		float angle;
		char moving;
		int axis[3];
		Uint32 t;
	public:
		float speed;
		float x, y;
		Enemy();
		Uint32 birth;
		void follow();
		void move();
		void show();
};

std::vector <Enemy*> enemy;

class Fruit {
	public:
		Fruit();
		float x, y;
		int type; // 1: speed; 2: slow;
		Uint32 birth;
		void check_life();
		void show();
};

std::vector <Fruit*> fruit;
PosMatrix fruitmatrix;

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

void turn_on_lights() {
	glEnable( GL_LIGHTING );
	glLightfv( GL_LIGHT1, GL_AMBIENT, LightAmbient );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, LightDiffuse );
	glLightfv( GL_LIGHT1, GL_POSITION, LightPosition );
	glEnable( GL_LIGHT1 );
}

void ground() {
	glPushMatrix();
	glTranslatef( -GRID_WIDTH/2, -GRID_HEIGHT/2, 0 );
	/*glBegin( GL_QUADS );
		glColor4f( 1, 0, 0, 1 );
		glVertex3f( 0, 		  0,		 0 );
		glVertex3f( GRID_WIDTH, 0, 		 0 );
		glVertex3f( GRID_WIDTH, GRID_HEIGHT, 0 );
		glVertex3f( 0, 		  GRID_HEIGHT, 0 );
	glEnd();*/
	glBegin( GL_LINES );
		int i;
		glColor4f( 0.2, 0.5, 1, 1 );
		for ( i = 0; i < 41; i++ ) {
			glVertex3f( i*L, 0, 0 );
			glVertex3f( i*L, GRID_HEIGHT, 0);
		}
		for ( i = 0; i < 31; i++ ) {
			glVertex3f( 0, i*L, 0 );
			glVertex3f( GRID_WIDTH, i*L, 0);
		}
	glEnd();
	glPopMatrix();
}

bool prob( float percent ) {
	percent *= 100;
	int p = rand() % ( 10001 );
	if ( p < percent ) return true;
		else return false;
}

// TODO: sistema questa cosa dei livelli
int level(){
	return score/200 + 1;
}

PosMatrix::PosMatrix() {
	for ( int i = 0; i < 40; i++ ) {
		for ( int j = 0; j < 30; j++ ) {
			matrix[i][j] = true;
		}
	}
}

int PosMatrix::get_x( int x ) {
	return ( x + GRID_WIDTH/2 ) / L;
}

int PosMatrix::get_y( int y ) {
	return ( y + GRID_HEIGHT/2 ) / L;
}

bool PosMatrix::is_free( int x, int y ) {
	return matrix[ get_x( x ) ][ get_y( y ) ];
}

void PosMatrix::set_free( int x, int y ) {
	matrix[ get_x( x ) ][ get_y( y ) ] = true;
}

void PosMatrix::set_busy( int x, int y ) {
	matrix[ get_x( x ) ][ get_y( y ) ] = false;
//	std::cout << "set busy: " << get_x(x) << "\t" << get_y(y) << "\n";
}

void PosMatrix::show() {
	for ( int i = 0; i < 40; i++ ) {
		for ( int j = 0; j < 30; j++ ) {
			if ( ! matrix[i][j] ) {
				glPushMatrix();
				glTranslatef( i*L-GRID_WIDTH/2, j*L-GRID_HEIGHT/2, 0 );
				glBegin( GL_QUADS );
					glColor4f( 0, 0, 1, 1 );
					glVertex3f( 0, 0, 0 );
					glVertex3f( L, 0, 0 );
					glVertex3f( L, L, 0 );
					glVertex3f( 0, L, 0 );
				glEnd();
				glPopMatrix();
			}
		}
	}
}

Pg::Pg() {
	x = y = 0;
	speed = 0.5;
	moving = 'n';
	angle = 0;
	axis[0] = axis[1] = axis[2] = 0;
}

void Pg::handle_input() {
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
}

void Pg::move() {
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
				score += enemy.size();
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
				score += enemy.size();
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
				score += enemy.size();
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
				score += enemy.size();
			}
			break;
		case 'n': 
			posmatrix.set_busy( x, y );
			break;
	}
}

void Pg::show() {
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

Enemy::Enemy() {
	do {
		x = ( rand() % 40 ) - 20;
		y = ( rand() % 30 ) - 15;
	} while ( ! posmatrix.is_free( x, y ) );
	x *= L;
	y *= L;
	speed = 0.5;
	moving = 'n';
	angle = 0;
	axis[0] = axis[1] = axis[2] = 0;
	birth = SDL_GetTicks();
	//next = NULL;
	//axis[1] = 1;
}

void Enemy::follow() {
	int n = rand() % 2;
	if ( moving == 'n' ) {
		if ( n ) {
			// controllo orizzontale
			if ( x > last_x ) {
				if ( posmatrix.is_free( x-L, y ) ) {
					posmatrix.set_busy( x-L, y );
					moving = 'l';
					axis[0] = 0;
					axis[1] = -1;
					axis[2] = 0;
				}
			} 
			if ( x < last_x ) {
				if ( posmatrix.is_free( x+L, y ) ) {
					posmatrix.set_busy( x+L, y );
					moving = 'r';
					angle = 90;
					axis[0] = 0;
					axis[1] = -1;
					axis[2] = 0;
				}
			} 
		} else {
			// controllo verticale
			if ( y > last_y ) {
				if ( posmatrix.is_free( x, y-L ) ) {
					posmatrix.set_busy( x, y-L );
					moving = 'd';
					axis[0] = 1;
					axis[1] = 0;
					axis[2] = 0;
				}
			}
			if ( y < last_y ) {
				if ( posmatrix.is_free( x, y+L ) ) {
					posmatrix.set_busy( x, y+L );
					moving = 'u';
					angle = 90;
					axis[0] = 1;
					axis[1] = 0;
					axis[2] = 0;
				}
			}
		}
	}
}

void Enemy::move() {
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

void Enemy::show() {
	glPushMatrix();
	glTranslatef( x, y, 0 );
	glRotatef( angle, axis[0], axis[1], axis[2] );
	glBegin( GL_QUADS );
		//front
		glColor4f( 1, 0, 0, 1 );
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

void create_enemy() {
	if ( enemy.size() == 0 ) {
		enemy.push_back( new Enemy );
	} else {
		// TODO: il numero dei nemici deve crescere in base al livello
		if ( ( SDL_GetTicks() - enemy.back()->birth ) > 8000 ) {
			if ( prob( .15 ) ) {
				enemy.push_back( new Enemy );
			}
		}
	}
}

void enemy_follow() {
//	enemy_top->item.follow();
	for ( int i = 0; i < enemy.size(); i++ ) {
		enemy[i]->follow();
	}
}

void enemy_move() {
//	enemy_top->item.move();
	for ( int i = 0; i < enemy.size(); i++ ) {
		enemy[i]->move();
	}
}

void enemy_show() {
//	enemy_top->item.show();
	for ( int i = 0; i < enemy.size(); i++ ) {
		enemy[i]->show();
	}
}

/*Fruit::Fruit() {
	birth = SDL_GetKeyState();
}

void create_fruit() {
	if ( fruit.size() == 0 ) {
		fruit.push_back( new Fruit );
	} else {
		if ( ( SDL_GetTicks() - fruit.back()->birth ) > 10000 ) {
			fruit.push_back( new Fruit );
			fruit.push_back( new Fruit );
			fruit.push_back( new Fruit );
			fruit.push_back( new Fruit );
			fruit.push_back( new Fruit );
		}
	}
}*/

int main( int argc, char *argv[] ) {
	srand48( time( NULL ) );
	bool quit = false;

	Pg pg;
	init();
	//turn_on_lights();

	// main loop
	while ( ( quit == false ) && ( ! you_lose ) ) {
		create_enemy();
		/////////////// EVENTI
		while( SDL_PollEvent( &event ) ) {
			if ( event.type == SDL_QUIT ) {
				quit = true;
			}
		}
		pg.handle_input();

		/////////////// LOGICA
		enemy_follow();

		/////////////// MOVIMENTO 
		pg.move();
		enemy_move();

		/////////////// RENDERING
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glPushMatrix();
			glRotatef( -(float)last_x/100, 0, 0, 5 );
			glRotatef( (float)last_y/100, 1, 0, 0 );
			ground();
			pg.show();
			enemy_show();
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
