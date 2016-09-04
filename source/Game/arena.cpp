class wall{
	public:
		bool edge[4];
};

class myarena{
	public:
		int N;
		wall** Arena;
		myarena(){
			N = 20;
			Arena = new wall*[N];
		    for(int i = 0; i < N; i++ ){
		                    Arena[i] = new wall[N];}

			for ( int i = 0; i < N; i++ ){
		        for ( int j = 0; j < N; j++ ) {
		            for ( int k = 0; k < 4; k++ )
		                Arena[i][j].edge[k] = 0;
		        }}
		    for ( int i = 0; i < N; i++ ){  Arena[0][i].edge[3] = 1;}
		    for ( int i = 0; i < N; i++ ){  Arena[i][0].edge[0] = 1;}
		    for ( int i = 0; i < N; i++ ) { Arena[i][N-1].edge[2] = 1;}
		    for ( int i = 0; i < N; i++ ) { Arena[N-1][i].edge[1] = 1;}
		    for ( int i = 5; i < N-5; i++){  Arena[4][i].edge[1] = 1;}
		    for ( int i = 5; i < 8; i++){  Arena[7][i].edge[1] = 1;   Arena[8][i].edge[1] = 1; }
		    for ( int i = 12; i < 15; i++){ Arena[7][i].edge[1] = 1; Arena[8][i].edge[1] = 1; }
		    for ( int i = 5; i < N-5; i++){ Arena[11][i].edge[1] = 1;}};};
