/*
 * enemy.h
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

class Enemy {
	private: 
		float angle;
		char moving;
		int axis[3];
		Uint32 t;
	public:
		float speed;
		float x, y;
		Enemy( PosMatrix &posmatrix );
		Uint32 birth;
		void follow( PosMatrix &posmatrix, std::vector <Enemy*> &enemy, int &last_x, int &last_y );
		void move( PosMatrix &posmatrix );
		void show();
};

void create_enemy( PosMatrix &posmatrix, std::vector <Enemy*> &enemy );

void enemy_follow( PosMatrix &posmatrix, std::vector <Enemy*> &enemy, int last_x, int last_y );

void enemy_move( PosMatrix &posmatrix, std::vector <Enemy*> &enemy );

void enemy_show( std::vector <Enemy*> &enemy );
