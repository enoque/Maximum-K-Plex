#include <bits/stdc++.h>
#define vi vector <int>
#define debug printf ("Chegou aqui!\n");
#define N 6
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

using namespace std;

vector <vi> grafo;
vector <vi> cores;
vector <int> tamanhoCores;
vector <int> vizinhos;
int matrizAdj[N+1][N+1];
	
void inicializa(){
	grafo.resize(N+1);
	cores.resize(N+1);
	vizinhos.resize(N+1);
	tamanhoCores.resize(N+1);
	for (int i = 0; i <= N; i++){
		cores[i].resize(N+1);
	}
	
	
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
}

//Complexidade O(n)
//Complexidade com lista será O(k)

bool pode_entrar (int c, int v){
	//bool flag = false;
	//if (v == 6 && c == 1) flag = true;
	for (int i = 0; i < (int) cores[c].size(); i++){
		if (cores[c][i]){
			if (matrizAdj[v][i]){
				//if (flag) printf ("u = %d\n", i);
				if (vizinhos[i] >= K-1){
					return false;
				}
			}
		}
	}
	return true;
}


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


li make_satured_list(li U, vi &nncnt, li s){
	int u = s.back();
	s.pop_back();
	li SL;
	
	for (int v : s){
		if (!grafo[u][v]){
			nncnt[v]++;
		}
	}
	
	for (int v : U){
		if (!grafo[u][v]) nncnt[v]++;
	}
	
	for (int v : s){
		if (nncnt[v] == K-1){
			SL.push_back(v);
		}
	}
	return SL;
}


bool isPlex(li SL, int v){
	for (int u : SL){
		if (!grafo[u][v]) return false;
	}
	return true;
}


li generation (li U, vi &nncnt, li s){
	li R;
	li SL = make_satured_list(U, nncnt, s);
	for (int v : U){
		if (nncnt[v] > K-1) continue;
		if (isPlex(SL, v)){
			R.push_back(v);
		}
	}
	return R;
}




void basic_plex(li s, li U, vi nncnt){
	if ((int)s.size() > s_max) s_max = s.size();
	
	while (U.size()){
		if (U.size() + s.size() <= (lu) s_max) return;
		int v = U.back();
		U.pop_back();
		s.push_back(v);
		vi newNncnt = nncnt;
		U = generation(U, newNncnt, s);
		basic_plex(s,U, newNncnt);
		s.pop_back();
	}
}



//Complexidade O(n*n*n) 
//Complexidade O(n*n*k)

vector<vi> co_k_coloaracao(){
	for (int i = 1; i <= N; i++){
		for (int c = 0; c < N; c++){
			if (pode_entrar(c,i)){
				printf ("c = %d v = %d\n", c, i);
				coloca_vertice_na_cor(c,i);
				tamanhoCores[c]++;
				break;
			}
		}
	}
	//printf("qtd = %lu\n", cores.size());
	return cores;
}


int main (){
	inicializa();
	co_k_coloaracao();
	vector <int> ordem;
	vector <int> limite_superior;
	
	int UB = 0;
	int grau_max_na_cor = 0;
	int graus[N+1];
	for (int i = 0; i < N; i++){
		memset(graus, 0, sizeof graus);
		grau_max_na_cor = 0;
		//printf ("cor = %d: ", i);
		int size = 0;
			
		for (int j = 1; j < (int) cores[i].size(); j++){
			//printf ("%d ", cores[i][j]);
			if (cores[i][j]){
				printf("cor %d entrou\n", i);
				ordem.push_back(j);
				size++;
				limite_superior.push_back( UB+ min(size, 2*K-2+(K%2) ));
				printf("vertice %d limite_superior %d\n", j, UB+ min(size, 2*K-2+(K%2)) );
				grau_max_na_cor = max (vizinhos[j], grau_max_na_cor);
				graus[vizinhos[j]]++;
			}
			
		}
		for (int j = 0; j <= N; j++){
			printf("%d = %d\n", j, graus[j]);
		}
		for (int j = N-1; j >= 0; j--){
			graus[j] += graus[j+1];
			
		}
		int max_m = 0;
		for (int j = 0; j <= N; j++){
			if (graus[j] > K+i) max_m = max(max_m, j);
		}
		printf("max_m = %d\n", max_m);
		
		UB += min(min(size,max_m+K), min(2*K-2+(K%2), grau_max_na_cor+K));
		printf ("\n");
	}
	for (int u: ordem) printf("%d ", u);
	printf("\n");
	printf ("vizinhos = %d\n", vizinhos[2]);
	printf ("tamanho = %d\n", tamanhoCores[0]);
	return 0;
}
