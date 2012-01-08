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
