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
