#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <vector>

/*Constant*/
#define MAX_VERTEX 20000
#define INT_SIZE 64
//#define DEBUG

#define is_edge(a,b) (bit[a][b/INT_SIZE]&(mask[b%INT_SIZE]))

typedef uint64_t word;

using namespace std;

int Vnbr, Enbr;          /* number of vertices/edges */
int clique[MAX_VERTEX]; /* table for pruning */
word bit[MAX_VERTEX][MAX_VERTEX/INT_SIZE+1];

vector <int> adj[MAX_VERTEX];

int pos[MAX_VERTEX];    /* reordering function */
int ord[MAX_VERTEX];    /*ordering by coloring*/
int set[MAX_VERTEX];    /* current clique */
int rec[MAX_VERTEX];	/* best clique so far */
int record;		/* weight of best clique */
int rec_level;          /* # of vertices in best clique */
int timeout;
int max_weight;
int cnt = 0;
long long int subp;
long long int oracle_calls;
int s = 2;

double elapsed;
clock_t clk;
word mask[INT_SIZE];

int fileerror();
int readFile(FILE * graphFile, int & n, int  & m);
void expand(int ct, int * table, int level, int * nncnt);


void degree_order(){
	int degree[MAX_VERTEX];
	bool used[MAX_VERTEX];
	int count;
	int delta = 0;
	int t = 0;

	/* order vertices */
	for(int i = 0; i < Vnbr; i++) {
		degree[i] = 0;			
		for(int j=0;j<Vnbr;j++){
  			if ( is_edge(i, j) ) {
				degree[i]++;
				t++;
			}
		}
		if ( degree[i] > delta) delta = degree[i];
  	}

	printf("t = %d, Enbr = %d\n", t, t/2);		

  	for(int i=0; i < Vnbr; i++) used[i] = false;

  	count = Vnbr - 1;

 	int max_degree, p;

  	while(count >= 0){

 		max_degree = -1;
 		p = -1;

 		for(int i = Vnbr-1; i >= 0; i--){
   			if(!used[i] && (degree[i] > max_degree)){
	 			max_degree = degree[i];
	  			p = i;
   			}
 		}

		//printf("degree[%d] = %d, pos[%d] = %d\n", count, max_degree, count, p);
 		pos[count--] = p;
 		used[p] = true;



 		for(int j=0;j<Vnbr;j++)
   			if((!used[j])&&(j!=p)&&(is_edge(p,j)))
	 			degree[j]--;

  	}
}

int main (int argc,char *argv[]){
	int i,j,k,p;
	int wth;
	FILE *infile;

	/* read input */
	if(argc < 4) {
		printf("Usage: RDPLEX timeout s infile\n");
		exit(1);
	}

	timeout = atoi( argv[1] );

  	printf("timeout %d s\n", timeout);

	s = atoi( argv[2] );

	if((infile=fopen(argv[3],"r"))==NULL)
		fileerror();


	/* initialize mask */
	mask[0] = 1LL;
	for(i=1;i<INT_SIZE;i++)
		mask[i] = mask[i-1]<<1;

	/* read graph */
	readFile(infile, Vnbr, Enbr);

	/* "start clock" */
	clk = clock();

	//degree_order();

	for(int i = 0; i < Vnbr; i++) pos[i] = i;

  	int nncnt[Vnbr];

	record = 0;
	for(int i = 0; i < Vnbr; i++){

		
		for(int j=0; j < Vnbr; j++){
			nncnt[j] = 0;
		}

		expand(i, pos, 0, nncnt);

		elapsed = ((double) (clock() - clk)) / CLOCKS_PER_SEC;

		printf("%d/%d (max %d) %.5f s\n", i+1, Vnbr, record, elapsed );

		clique[pos[i]] = record;
	}

	//expand(Vnbr, pos, 0, nncnt);

	elapsed = ((double) (clock() - clk)) / CLOCKS_PER_SEC;

	printf("best clique: ");
	for(int i = 0; i < record; i++){
		printf("%d ", rec[i]+1);
	}

	printf("\n");
	printf("subp                 = %10lld\n", subp);
	printf("record               = %10d\n", record);
	printf("time                 = %10.5f\n", elapsed);

	return 0;
}


void make_saturated_list(int * set, int size, int * table, int ct, int * nncnt, int * saturated_vertex, int & saturated_size){
	int u = set[size-1];
	saturated_size = 0;

	for(int j = 0; j < size-1; j++){
    		int v = set[j];

    		if( !is_edge(u,v) ){
      			nncnt[v]++;
    		}
  	}

	for(int j = 0; j < ct; j++){
    		int v = table[j];

    		if( !is_edge(u,v) ){
      			nncnt[v]++;
    		}
  	}

	for(int j = 0; j < size; j++){
		int v = set[j];
		if( nncnt[v] == s-1){
      			saturated_vertex[saturated_size++] = v;
    		}

  	}
}



bool is_plex(int * set, int size, int * saturated_vertex, int saturated_size, int v){
	for(int i = 0; i < saturated_size; i++){
	    	int u = saturated_vertex[i];

    		if( !is_edge(u,v) ) return false;
  	}
  	return true;
}


void expand(int ct, int * table, int level, int * nncnt){

	int newnncnt[Vnbr];
	int saturated_vertex[Vnbr];
	int saturated_size;
	int newtable[Vnbr];
	int newct;

	subp++;

	if(++cnt > 1000) {
		cnt = 0;
		elapsed = ((double) (clock() - clk)) / CLOCKS_PER_SEC;
		if(elapsed >= timeout) {
			printf("TIMEOUT\n");
			printf("best clique: ");
			for(int i = 0; i < record; i++){
				printf("%d ", rec[i]+1);
			}

			printf("\n");
			printf	("record          =  %10d\n", record );
			printf("subp            =  %10lld\n", subp );
			printf("time            =  %10.5f\n", elapsed );
			exit(0);
		}
  	}

  	if( ct <= 0){
    		if(ct == 0){
      			set[level++] = table[0];
    		}

    		if(level > record ){
      			record = level;
      			printf("kplex size %d  encontrado em %lf\n", record, (double) (clock() - clk) / CLOCKS_PER_SEC);
      			printf("[");
      			for(int i = 0; i < level; i++){
        			rec[i] = set[i];
        			printf("%d ", rec[i]+1);
      			}
      			printf("]\n");

		}
	}



	for(int i = ct; i >= 0; i--){

		if( i < ct && level == 0) return ;

		if( i <= record - level) return ;

		int v = table[i];

		if((level>0)&&(clique[v]<=(record-level))) return;

		set[level] = v;

		for(int j = 0; j < Vnbr; j++) newnncnt[j] = nncnt[j];

		make_saturated_list(set, level+1, table, i, newnncnt, saturated_vertex, saturated_size);

		newct = 0;

		for(int j = 0; j < i; j++){

	      		int v = table[j];

	      		if( newnncnt[v] > s - 1) continue;

	      		if( is_plex(set, level+1, saturated_vertex, saturated_size, v) ) newtable[newct++] = v;
	  	}

	  	if(newct + level+1 <= record) continue;

	  	expand(newct-1, newtable, level+1, newnncnt);

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


  	for(i=0;i<n;i++)
		for(j=0;j<n/INT_SIZE+1;j++)
  			bit[i][j] = 0;

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
				bit[i][j/INT_SIZE] |= mask[j%INT_SIZE];
				bit[j][i/INT_SIZE] |= mask[i%INT_SIZE];
        			adj[i].push_back(j);
        			adj[j].push_back(i);

			}
		}
		type = fgetc(graphFile);
	}

	int contador = 0;

	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if( is_edge(i,j) ) contador++;
		}
	}

	contador = contador / 2;


	if( contador != m ){
		printf("Error bit adjacency graph representation\n");
		exit(1);
	}

	return 0;
}


int fileerror(){
	printf("Error in graph file\n");
	exit(0);
}
