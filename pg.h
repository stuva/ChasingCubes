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
