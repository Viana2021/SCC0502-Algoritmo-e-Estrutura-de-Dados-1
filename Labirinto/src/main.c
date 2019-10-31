#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <stack.h>
#include <math.h>
#include <defines.h>

int main(void){
	int np; //Numero de pontos
	int nc; //Numero de camaras
	int ns; //Numero de segmentos
	int s_in, s_out; //Quais pontos estao ligados, o de entrada e saida
	int start_index;
	float x, y;
	int indice;
	int isExit;
	GRAPH *graph;
	graph = (GRAPH *) calloc(1, sizeof(GRAPH));
	GRAPH_ELEM **graph_elem;

	scanf("%d", &np);
	graph->adj = (int **) malloc(np * sizeof(int *));
	graph->vertices = np;
	graph_elem = (GRAPH_ELEM **) malloc(np * sizeof(GRAPH_ELEM *));

	//Armazenando todos os pontos
	for(i = 0; i < np; i++){
		graph->adj[i] = (int *) calloc(np, sizeof(int));
		graph_elem[i] = (GRAPH_ELEM *) calloc(1, sizeof(GRAPH_ELEM));
		scanf("%f", &x);
		scanf("%f", &y);
		graph_elem[i]->x = x;
		graph_elem[i]->y = y;
	}

	//Armazenando todas as camaras
	scanf("%d", &nc);
	if(nc > np) return INVALID_CAM_NUMBER;
	for (i = 0; i < nc; i++){
		scanf("%d", &indice);
		if(indice > np) return INVALID_INDEX;
		graph_elem[indice]->isCam = TRUE;
		scanf("%d", &isExit);
		if(isExit) graph_elem[indice]->isExit = TRUE;
		else graph_elem[indice]->isExit = FALSE;

	}

	//Armazenando todos os paths
	scanf("%d", &ns);
	for (i = 0; i < ns; i++){
		scanf("%d %d", &s_in, &s_out);
		if(s_in > np || s_out > np || s_in == s_out) return INVALID_PATH;
		if(s_in < 0 || s_out < 0){
			graph->adj[s_in][s_out] = 0;
			graph->adj[s_out][s_in] = 0;
		}
		else{
			graph->adj[s_in][s_out] = 1;
			graph->adj[s_out][s_in] = 1;
		}
	}

	//Lendo indice da camara de inicio
	scanf("%d", &start_index);
	graph->start_index = start_index;

	return SUCCESS;
}
