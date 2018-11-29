#include <bits/stdc++.h>
#define vi vector <int>
#define li list <int>
#define K 2
#define N 4100
#define ms(v, x) memset(v, x, sizeof(v))
#define llu unsigned long long int
using namespace std;
vector <vi> G;
int matrizAdj[4100][4100];
unsigned int smax;
int n, m;
vector <vi> cores;
vi tamanhoCores;
int vizinhos[N+1];
int graus[N+1];
bool pertence_ao_grafo[N+1];
clock_t clk;
int timeout;
void inicializa(){
	G.resize(10);
	
	matrizAdj[1][2] = 1;
	matrizAdj[2][1] = 1;
	G[1].push_back(2);
	G[2].push_back(1);
	
	
	matrizAdj[1][5] = 1;
	matrizAdj[5][1] = 1;
	G[1].push_back(5);
	G[1].push_back(5);
	
	matrizAdj[1][3] = 1;
	matrizAdj[3][1] = 1;
	G[1].push_back(3);
	G[3].push_back(1);
	
	
	matrizAdj[3][4] = 1;
	matrizAdj[4][3] = 1;
	G[3].push_back(4);
	G[4].push_back(3);
	
	matrizAdj[1][4] = 1;
	matrizAdj[4][1] = 1;
	G[1].push_back(4);
	G[4].push_back(1);
	
	//matrizAdj[5][3] = 1;
	//matrizAdj[3][5] = 1;
	//G[3].push_back(5);
	//G[5].push_back(3);
}



li make_saturated_list (li U, vi &nncnt, li S){
	li SL;
	if (!S.size()) return li();
	int u = S.back();
	for (int v : S){
		if (!matrizAdj[v][u] && u != v)
			nncnt[v]++;
	}
	for (int v : U){
		if (!matrizAdj[v][u] && u != v)
			nncnt[v]++;
	}
	
	for (int v: S){
		if (nncnt[v] == K-1)
			SL.push_back(v);
	}
	
	return SL;
}


bool isPlex (li SL, int v){
	for (int u : SL){
		if (!matrizAdj[u][v])
			return false;
	}
	return true;
}


void generation (li U, li S, li &newU, vi &nncnt){
	newU.clear();
	li SL = make_saturated_list(U, nncnt, S);	
	
	for (int v : U){
		if (nncnt[v] >= K)
			continue;
		
		if (isPlex(SL, v))
			newU.push_back(v);
		
	}
}

void basicPlex (li S, li U, vi nncnt){
	if (S.size() > smax){
		smax = S.size();
		printf("ENCONTROU UMA SOLUCAO DE TAMANHO %d  WITH BASIC!\n", smax);
	}
	li newU;
	
	while (U.size()){
		if (U.size() + S.size() <= smax) return;
		int v = U.back();
		U.pop_back();
		S.push_back(v);
		vi newNncnt = nncnt;
		generation(U, S, newU, newNncnt);
		basicPlex (S, newU, newNncnt);
		S.pop_back();
	}
}




bool pode_entrar (int c, int v){
	for (int u : cores[c]){
		if (matrizAdj[v][u] && vizinhos[u] >= K-1)
			return false;
	}
	
	return true;
}



void coloca_vertice_na_cor (int c, int v){
	int contador = 0;
	for (int u : cores[c]){
		if (matrizAdj[u][v]){
			vizinhos[u]++;
			contador++;
		}
	}
	cores[c].push_back(v);
	vizinhos[v] = contador;
}

vector<vi> co_k_coloaracao(bool pertence_ao_grafo[N+1]){
	bool flag;
	for (int i = 1; i <= n; i++){
		if (!pertence_ao_grafo[i]) continue;
		flag = false;
		for (int c = 0; c < (int)cores.size(); c++){
			if (pode_entrar(c,i)){
				coloca_vertice_na_cor (c, i);
				flag = true;
				break;
			}
		}
		if (!flag){
			cores.push_back(vi());
			coloca_vertice_na_cor (cores.size()-1, i);
		}
	}
	return cores;
}




li ordem;
li limite_superior;

void gera_limite(li U, int minimo, li &newU, li &limites){
	newU.clear();
	limites.clear();
	//ordem.clear();
	ms(vizinhos,0);
	ms(pertence_ao_grafo, 0);
	//for (auto &c: cores)
		//c.clear();
	cores.clear();
	for (int u : U) pertence_ao_grafo[u] = true;
	co_k_coloaracao(pertence_ao_grafo);
	//limite_superior.clear();
	//tamanhoCores.clear();
	int UB = 0;
	int grau_max_na_cor = 0;
	ms(graus,0);
	for (int i = 0; i < (int) cores.size(); i++){
		memset(graus, 0, sizeof graus);
		grau_max_na_cor = 0;
		int size = 0;
			
		for (int v : cores[i]){
			if (!pertence_ao_grafo[v]) continue;
			//ordem.push_back(v);
			newU.push_back(v);
			
			size++;
			grau_max_na_cor = max(vizinhos[v], grau_max_na_cor);
			graus[vizinhos[v]]++;
		}
		int max_m = 0x3f3f3f3f;
		/*
		for (int j = N-1; j >= 0; j--){
			graus[j] += graus[j+1];
			
		}
		
		for (int j = 0; j <= N; j++){
			if (graus[j] > K+i) max_m = max(max_m, j);
		}*/
		int cont = size-1;
		for (int v : cores[i]){
			if (!pertence_ao_grafo[v]) continue;
			//if (UB+min(min(size-cont,max_m+K), min(2*K-2+(K%2), grau_max_na_cor+K)) >= minimo){
				//limite_superior.push_back(UB+min(min(size-cont,max_m+K), min(2*K-2+(K%2), grau_max_na_cor+K)));
				limites.push_back(UB+min(min(size-cont,max_m+K), min(2*K-2+(K%2), grau_max_na_cor+K)));
				//ordem.push_back(v);
				cont--;
				//printf("xxx\n");
			//}
			//printf("V = %d   limite = %d\n", v, limite_superior.back());
		}
		
		
		UB += min(min(size,max_m+K), min(2*K-2+(K%2), grau_max_na_cor+K));
		//printf ("\n");
	}
}












int cont = 0;
bool flag = false;
llu subproblemas = 0;
void basicPlexWithCoKColoracao (li S, li U, vi nncnt){
	if (S.size() > smax){
		smax = S.size();
		printf("ENCONTROU UMA SOLUCAO DE TAMANHO %d  em %lfs\n", smax, (double)(clock()-clk)/CLOCKS_PER_SEC);
	}
	if (flag) return;
	subproblemas++;
	li newU;
	li limites;
	gera_limite(U, smax-S.size(), U, limites);
	
	
	if ((double) (clock() - clk) / CLOCKS_PER_SEC > timeout){
		flag = true;
		return;
	}
	
	
	while (U.size()){
		if (flag) return;
		if (U.size() + S.size() <= smax) return;
		int limite = limites.back();
		limites.pop_back();
		if (limite + S.size() <= smax){return;}
		int v = U.back();
		U.pop_back();
		S.push_back(v);
		vi newNncnt = nncnt;
		generation(U, S, newU, newNncnt);
		basicPlexWithCoKColoracao (S, newU, newNncnt);
		S.pop_back();
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

	G.resize(n+1);
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
				G[i].push_back(j);
				G[j].push_back(i);
				}

		}
		type = fgetc(graphFile);
	}
	return 0;
}



int main (int argc,char *argv[]){
	FILE *arquivo;
	
	if(argc < 2) {
    		printf("Usage: BB [timeout] infile\n");
    		exit(1);
  	}

  	timeout = atoi( argv[1] );

  	printf("timeout %d s\n", timeout );

  	if((arquivo=fopen(argv[2],"r"))==NULL){
		printf("Error in graph file\n %s\n", argv[2]);
		exit(0);
  	}
	
	readFile(arquivo, n, m);
	smax = 0;
	//n = 5;
	li U,S;
	vi nncnt;
	nncnt.resize(n+1);
	for (int i = 1; i <= n; i++) U.push_back(i);
	clk = clock();
	basicPlexWithCoKColoracao(S,U,nncnt);
	printf("Tempo total de  %lfs !!\n", (double)(clock()-clk)/CLOCKS_PER_SEC);
	printf("Total de subproblemas = %llu\n", subproblemas);
	//printf("maior = %u\n", smax);
	
	
	return 0;
}
