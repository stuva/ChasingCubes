/*
 * pg.h
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
		void handle_input( PosMatrix &posmatrix, bool &you_lose);
		void move( PosMatrix &posmatrix);
		void show( int &last_x, int &last_y );
};
