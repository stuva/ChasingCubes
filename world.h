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

void ground();

bool prob( float percent );
