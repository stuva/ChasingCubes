#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "define.h"
#include "world.h"

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

bool prob( float percent ) {
	percent *= 100;
	int p = rand() % ( 10001 );
	if ( p < percent ) return true;
		else return false;
}

