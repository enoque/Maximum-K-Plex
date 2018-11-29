#include <bits/stdc++.h>
#define vi vector <int>
#define li list <int>
#define K 2
#define llu unsigned long long int
using namespace std;
vector <vi> G;
int matrizAdj[4100][4100];
unsigned int smax;
int n, m;
int saiu [4100];
clock_t clk;
int timeout;
llu subproblemas = 0;
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
		printf("ENCONTROU UMA SOLUCAO DE TAM =  %u\n", smax);
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


vector<vi> s;
li branching (li U, int limite){
	int UB = limite;
	int j = 0;
	li newU;
	s.clear();
	while (U.size()){
		//printf("UB = %d\n", UB);
		if (UB <= 0)
			return U;
		memset(saiu, 0, sizeof saiu);
		s.push_back(vi());
		li R = U;
		for (int v : R){
			if (saiu[v]) continue;
			s[j].push_back(v);
			for (int u : G[v]){
				saiu[u] = 1;
			}
			saiu[v] = 1;
		}
		int count = min ((int)s[j].size(), min(K, UB));
		UB -= count;
		for (int i = 0; i < count; i++)
			U.remove(s[j][i]);
		j++;
	}
	return U;
}


bool flag = false;
void basicPlexWithBranching (li S, li U, vi nncnt){
	if (S.size() > smax){
		smax = S.size();
		printf("ENCONTROU UMA SOLUCAO DE TAMANHO %d  em %lfs\n", smax, (double)(clock()-clk)/CLOCKS_PER_SEC);
	}
	li newU;
	subproblemas++;
	if ((double) (clock() - clk) / CLOCKS_PER_SEC > timeout){
		flag = true;
		return;
	}
	
	li R = branching(U, smax-S.size());
	while (R.size()){
		if (flag) return;
		if (U.size() + S.size() <= smax) return;
		int v = U.back();
		U.pop_back();
		R.pop_back();
		S.push_back(v);
		vi newNncnt = nncnt;
		generation(U, S, newU, newNncnt);
		basicPlexWithBranching (S, newU, newNncnt);
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
  	
	smax = 0;
	n = 5;
	//inicializa();
	readFile(arquivo, n, m);
	li U,S;
	vi nncnt;
	nncnt.resize(n+1);
	for (int i = 1; i <= n; i++) U.push_back(i);
	clk = clock();
	basicPlexWithBranching(S,U,nncnt);
	printf("maior = %u\n", smax);
	printf("Tempo total %lfs !!\n", (double)(clock()-clk)/CLOCKS_PER_SEC);
	printf("Total de subproblemas = %llu\n", subproblemas);
	return 0;
}
