#include <bits/stdc++.h>
#define vi vector <int>
#define debug printf ("Chegou aqui!\n");
#define N 5000
#define K 2
#define llu long long unsigned
#define lu long unsigned
#define lld long long int
#define ii pair<int, int>
#define x first
#define y second
#define pb(x) push_back(x)
#define go(i,n) for (int i = 0; i < (int)n; i++)
#define vi vector <int>
#define vii vector <ii>
#define INF 0x3f3f3f3f
#define ff first
#define ss second
#define dd pair<double, double>
#define ms(v, x) memset(v, x, sizeof(v))
#define li list<int>
clock_t clk;
using namespace std;

vector <vi> grafo;
vector <vi> cores;
vi tamanhoCores;
vi vizinhos;
int matrizAdj[N+1][N+1];
int n,m;


int k, timeout;

typedef struct Solucao{
	li s;
	int vertices[N+1];
}Solucao;

void inicializa(){
	
	grafo.resize(N+1);
	
	cores.resize(N+1);
	
	vizinhos.resize(N+1);
	
	tamanhoCores.resize(N+1);
	
	for (int i = 0; i <= N; i++){
		cores[i].resize(N+1);
	}
	
	
	/*
	grafo[1].push_back(2);
	grafo[1].push_back(3);
	grafo[1].push_back(6);
	grafo[2].push_back(6);
	grafo[3].push_back(4);
	grafo[4].push_back(5);
	grafo[5].push_back(6);
	
	// ##########  Voltando #########
	
	grafo[2].push_back(1);
	grafo[3].push_back(1);
	grafo[6].push_back(1);
	grafo[6].push_back(2);
	grafo[4].push_back(3);
	grafo[5].push_back(4);
	grafo[6].push_back(5);
	
	// ########## matriz #########
	matrizAdj[1][2] = 1;
	matrizAdj[2][1] = 1;
	matrizAdj[1][3] = 1;
	matrizAdj[3][1] = 1;
	matrizAdj[1][6] = 1;
	matrizAdj[6][1] = 1;
	matrizAdj[2][6] = 1;
	matrizAdj[6][2] = 1;
	matrizAdj[3][4] = 1;
	matrizAdj[4][3] = 1;
	matrizAdj[4][5] = 1;
	matrizAdj[5][4] = 1;
	matrizAdj[5][6] = 1;
	matrizAdj[6][5] = 1;
	
	
	
	// #################
	
	grafo[4].push_back(1);
	grafo[1].push_back(4);
	matrizAdj[1][4] = 1;
	matrizAdj[4][1] = 1;
	
	matrizAdj[2][4] = 1;
	matrizAdj[4][2] = 1;
	matrizAdj[5][2] = 1;
	matrizAdj[2][5] = 1;
	*/
	
}


bool pertence (li s, int v){
	for (int x : s)
		if (x == v) return true;
		
	return false;
}








//Complexidade O(n)
//Complexidade com lista será O(k)

/*
Esta função testa se o vertice v pode receber a cor c no grafo representando
pela matrizAdj.
Para cada vertice i na cor c que eh vizinho de v, verificamos se 
o numero de vizinhos do vertice i na cor c é menor  K-1.
 
*/





bool pode_entrar (int c, int v){
	for (int i = 0; i < (int) cores[c].size(); i++){
		if (cores[c][i]){
			if (matrizAdj[v][i]){
				if (vizinhos[i] >= K-1){
					return false;
				}
			}
		}
	}
	return true;
}

/*
Esta funcao coloca o vertice v na cor c. Alem disso, atualiza o vetor
vizinhos para todo vertice na cor c que são vizinhos de v.
*/

void coloca_vertice_na_cor (int c, int v){
	
	int contador = 0;
	for (int i = 0; i < (int) cores[c].size(); i++){
		if (cores[c][i]){
			if (matrizAdj[v][i]){
				vizinhos[i]++;
				contador++;
			}
		}
	}
	
	cores[c][v] = 1;
	vizinhos[v] = contador;
}

int s_max = 0;

/*

Esta funcao recebe a matriz de Adjacência e um vetor caracteristico 
do vertices de um subgrafo. Devolve um vector de vi representando uma 
co-k-coloracao.
*/

//Complexidade O(n*n*n) 

vector<vi> co_k_coloaracao(int pertence_ao_grafo[N+1]){
	for (int i = 1; i <= n; i++){
		if (!pertence_ao_grafo[i]) continue;
		for (int c = 0; c < n; c++){
			if (pode_entrar(c,i) && pertence_ao_grafo[i]){
				//printf ("c = %d v = %d\n", c, i);
				coloca_vertice_na_cor(c,i);
				tamanhoCores[c]++;
				break;
			}
		}
	}
	//printf("qtd = %lu\n", cores.size());
	return cores;
}


/*

*/


li make_satured_list(li U, vi &nncnt, li s){
	int u = s.back();
	//printf("u = %d\n", u);
	s.pop_back();
	if (u == 0){
		return list<int>();
	}
	li SL;
	
	for (int v : s){
		if (!matrizAdj[u][v]){
			nncnt[v]++;
		}
	}
	
	for (int v : U){
		if (!matrizAdj[u][v] && u != v) nncnt[v]++;
	}
	
	for (int v : s){
		//printf("%d = %d\n", v, nncnt[v]);
		if (nncnt[v] == K-1 && v != 0){
			SL.push_back(v);
		}
		
	}
	/*printf ("SOLUCAO = ");
	for (int x : s) printf("%d ", x);
	printf("\n");
	
	printf ("VETOR = ");
	for (int x : nncnt) printf("%d ", x);
	printf("\n");
	printf("saiu = %lu\n", SL.size());*/
	
	
	return SL;
}


bool isPlex(li SL, int v){
	//printf("tam = %lu\n", SL.size());
	//for (int u: grafo[v]) printf("x%d ", u);
	//printf("\n");
	
	for (int u : SL){
		if (!matrizAdj[u][v]) {return false;}
	}
	return true;
}


li generation (li U, vi &nncnt, li s){
	li R;
	li SL = make_satured_list(U, nncnt, s);
	//printf("tam list = %lu\n", U.size());
	//for (int u : U) printf("%d ", u);
	//printf("\n");
	/*printf("tam SL = %lu   s = %lu\n", SL.size(), s.size());
	printf("SL\n");
	for (int x : SL)
		printf("%d ", x);
	printf("\n");
	printf("s\n");
	for (int x : s)
		printf("%d ", x);
	printf("\n");
	*/
	for (int v : U){
		//printf("%d ", v);
		if (nncnt[v] > K-1){continue;}
		if (isPlex(SL, v)){
			R.push_back(v);
		}
		else{
			//printf("x\n");
			//debug;
		}
	}
	//printf("\n");
	//printf("tam gerado = %lu\n", R.size());
	return R;
}


li ordem;
li limite_superior;
	

void gera_limite(int pertence_ao_grafo[N+1]){
	for (vi &x : cores)
		for (int &xx : x) xx = 0;
	ordem.clear();
	vizinhos.clear();
	co_k_coloaracao(pertence_ao_grafo);
	limite_superior.clear();
	tamanhoCores.clear();
	int set[N+1];
	ms(set,0);
	int UB = 0;
	int grau_max_na_cor = 0;
	int graus[N+1];
	for (int i = 0; i < N; i++){
		memset(graus, 0, sizeof graus);
		grau_max_na_cor = 0;
		//printf ("cor = %d: ", i);
		int size = 0;
			
		for (int j = 1; j < (int) cores[i].size(); j++){
			if (!pertence_ao_grafo[j]){continue;}
			if (set[j]) continue;
			//printf ("%d ", cores[i][j]);
			if (cores[i][j]){
				set[j] = 1;
				//printf("cor %d entrou\n", i);
				ordem.push_back(j);
				size++;
				limite_superior.push_back( UB+ min(size, 2*K-2+(K%2) ));
				//printf("vertice %d limite_superior %d\n", j, UB+ min(size, 2*K-2+(K%2)) );
				grau_max_na_cor = max (vizinhos[j], grau_max_na_cor);
				graus[vizinhos[j]]++;
			}
			
		}
		/*for (int j = 0; j <= N; j++){
			printf("%d = %d\n", j, graus[j]);
		}*/
		for (int j = N-1; j >= 0; j--){
			graus[j] += graus[j+1];
			
		}
		int max_m = 0;
		for (int j = 0; j <= N; j++){
			if (graus[j] > K+i) max_m = max(max_m, j);
		}
		//printf("max_m = %d\n", max_m);
		
		UB += min(min(size,max_m+K), min(2*K-2+(K%2), grau_max_na_cor+K));
		//printf ("\n");
	}
	//for (int u: ordem) printf("%d ", u);
	//printf("\n");
	//printf ("vizinhos = %d\n", vizinhos[2]);
	//printf ("tamanho = %d\n", tamanhoCores[0]);
}
bool flag = false;

void basic_plex_with_limite(li s, Solucao U, vi nncnt, int cont){
	//printf("CHEGOU COM %lu!!\n", s.size());
	if ((int)s.size() > s_max) {
		s_max = s.size();
		printf("resposta atual = %lu  achado no tempo = %lf\n", s.size(), ((double) (clock() - clk)) / CLOCKS_PER_SEC);
		//for (int x : s)
			//printf("%d ", x);
		//printf("\n");
		//if (s.size() == 3) cont = 2;
	}
	if ((int) (clock() - clk) / CLOCKS_PER_SEC > timeout){
		flag = true;
		return;
	}
	//if (U.s.size() == 0) return;
	ms(U.vertices,0);
	for (int x : U.s) U.vertices[x] = 1;
	gera_limite(U.vertices);
	U.s = ordem;
	//for (int x : ordem)
	//	printf("%d ", x);
	//printf("\n");
	/*if (cont == 200){
		printf("tam ordem = %lu  s = %lu\n", ordem.size(), s.size());
		for (int x : ordem) printf("%d ", x);
		printf("\n");
		printf("tam ordemXX = %lu  s = %lu\n", ordem.size(), s.size());
		for (int x : s) printf("%d ", x);
		printf("\n");
	}*/
	li limiteVertices = limite_superior;
	
	//cout << U.s.size() << endl;
	//for (int u : nncnt) printf("%d ", u);
	//printf("\n");
	
	
	//printf("novo\n");
	while (U.s.size()){
		if(flag) return;
		//flag = true;
		//printf("size U = %lu\n", U.s.size());
		//printf("TAM = %lu  s_max = %d\n", U.s.size(), s_max);
		if (U.s.size() + s.size() <= (lu) s_max) return;
		int v = U.s.back();
		int limite = limiteVertices.back();
		if (limite + (int)s.size() <= s_max) return;
		U.s.pop_back();
		/*for (int i = 1; i <= N; i++){
			if (matrizAdj[v][i]) U.matrizAdj[i][v] = 0;
			U.matrizAdj[v][i] = 0;
		}*/
		U.vertices[v] = 0;
		//s.push_back(v);
		vi newNncnt = nncnt;
		Solucao newU = U;
		li newS = s;
		newS.push_back(v);
		newU.s = generation(U.s, newNncnt, newS);
		//printf("Antigo U = %lu  novo = %lu  vertice_tirado = %d  cont = %d\n", U.s.size(), newU.s.size(), v, cont);
		/*if (cont == 1 && v == 4){
			//flag = true;
			//printf("#######\n");
			//for (int x : newU.s)
				//printf("%d ", x);
			//printf("\n");
			//printf("diff!\n");
			//for (int x : nncnt)
				//printf("%d ", x);
			//printf("\n");
			//printf("diff!\n");
			for (int x : newNncnt)
				printf("%d ", x);
			printf("\n");
			//return;
		}*/
		//printf("s1 = %lu\n", s.size());
		
		//printf("newU = %lu\n", newU.s.size());	
		//printf("####\n");
		//printf("tamNewS = %lu\n", newS.size());
		basic_plex_with_limite(newS, newU, newNncnt, cont+1);
		//s.pop_back();
		
		//printf("s2 = %lu\n", s.size());
		//basic_plex(s,U, newNncnt);
		//s.pop_back();
		
	}
}

int readFile(FILE * graphFile, int & n, int  & m) {
	char type  = ' ';
	char linestr[1024];
	char * datastr;
	long i, j;
	int nedges;

	while (type != 'p') {
		type = fgetc(graphFile);
		if (type != EOF) {
			/* header */
			if (type == 'c') {
				datastr = fgets(linestr, 1024, graphFile);
				if (datastr == NULL)
					return -1;
			}

			/* Vertices */
			if (type == 'p') {
				datastr = fgets(linestr, 1024, graphFile);
				if (datastr == NULL)
					return -1;

				datastr = strtok(linestr," ");

				datastr = strtok(NULL," ");
				n = atoi(datastr);

				datastr = strtok(NULL," ");
				m = atoi(datastr);

			}
		}
	}

	printf("Graph with %d vertices and %d edges density %f\n", n, m, (double)(2*m)/(n*(n-1)) );


  	nedges = 0;
	type = fgetc(graphFile);
	while (type != EOF) {
		/* Edges */
		if (type == 'e') {
			datastr = fgets(linestr, 100, graphFile);


			if (datastr == NULL)
				return -1;

			datastr = strtok(linestr," ");
			i = atol(datastr) - 1;

			datastr = strtok(NULL," ");
			j = atol(datastr) - 1;

			//printf("edge %d %d\n", i , j);



  			if( i != j ){
				nedges++;
				matrizAdj[i][j] = 1;
				matrizAdj[j][i] = 1;

				}

		}
		type = fgetc(graphFile);
	}

	return 0;
}


int main (int argc,char *argv[]){
	FILE *arquivo;
	
	
	if(argc < 4) {
		printf("Usage: RDPLEX timeout s infile\n");
		exit(1);
	}

	timeout = atoi( argv[1] );

  	printf("timeout %d s\n", timeout);

	k = atoi( argv[2] );

	if((arquivo=fopen(argv[3],"r"))==NULL){
		printf("Arquivo não encontrado!!\n");
		return 0;
	}
	
	//n = 6;
	//arquivo = fopen("brock200_1.clq", "r");
	readFile(arquivo, n, m);
	inicializa();
	
	
	
	li s;
	s.push_back(0);
	Solucao U;
	/*for (int i = 1; i <= N; i++){
		U.vertices[i] = 1;
		for (int j = 0; j <= N; j++){
			U.matrizAdj[i][j] = matrizAdj[i][j];
		}
	}*/
	for (int i = 1; i <= n; i++){
		U.s.push_back(i);
	}
	
	s_max = 0;
	vi x;
	x.resize(N+1, 0);
	clk = clock();
	basic_plex_with_limite(s,U,x, 0);
	
	printf("tempo gasto = %10lf\n", ((double) (clock() - clk)) / CLOCKS_PER_SEC);
	
	
	//gera_limite();
	printf("Maior k-plex = %d\n", s_max-1);
	return 0;
}
