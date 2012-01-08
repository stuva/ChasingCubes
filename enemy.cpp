#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "vector"

#include "define.h"
#include "world.h"
#include "enemy.h"

Enemy::Enemy( PosMatrix &posmatrix ) {
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

void Enemy::follow( PosMatrix &posmatrix, std::vector <Enemy*> &enemy, int &last_x, int &last_y ) {
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

void Enemy::move( PosMatrix &posmatrix ) {
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

void create_enemy( PosMatrix &posmatrix, std::vector <Enemy*> &enemy ) {
	if ( enemy.size() == 0 ) {
		enemy.push_back( new Enemy( posmatrix ) );
	} else {
		// TODO: il numero dei nemici deve crescere in base al livello
		if ( ( SDL_GetTicks() - enemy.back()->birth ) > 8000 ) {
			if ( prob( .15 ) ) {
				enemy.push_back( new Enemy( posmatrix ) );
			}
		}
	}
}

void enemy_follow( PosMatrix &posmatrix, std::vector <Enemy*> &enemy, int last_x, int last_y ) {
	for ( int i = 0; i < enemy.size(); i++ ) {
		enemy[i]->follow( posmatrix, enemy, last_x, last_y );
	}
}

void enemy_move( PosMatrix &posmatrix, std::vector <Enemy*> &enemy ) {
	for ( int i = 0; i < enemy.size(); i++ ) {
		enemy[i]->move( posmatrix );;
	}
}

void enemy_show( std::vector <Enemy*> &enemy ) {
	for ( int i = 0; i < enemy.size(); i++ ) {
		enemy[i]->show();
	}
}


