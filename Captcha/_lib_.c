#include <stdio.h>
#include <stdlib.h>
#include <_lib_.h>

//Funcao para alocar matriz de int
//feita para evitar codigo macarronico
int **alocMatrix(int largura, int altura){
	int **matriz;
	int j;
	matriz = (int **) malloc(altura * sizeof(int *));
	if(!matriz)
		return (int **) ALOCATION_ERROR;
	for(j = 0; j < altura; j++){
		matriz[j] = (int *) malloc(largura * sizeof(int));
		if(!matriz[j])
			return (int **) ALOCATION_ERROR;
	}
	return matriz;
}

//Funcao para alocar string dinamicante
//feita para evitar codigo macarronico
char *alocString(){
	char *string;
	string = (char *) calloc(30, sizeof(char));
	return string;
}

//Funcao para ponteiro do tipo file apontar para o arquivo para sua posterior leitura
FILE *createFile(){
	FILE *file;
	char *arquivo;
	arquivo = alocString();
	if(!arquivo)
		return (FILE *) ALOCATION_ERROR;

	scanf(" %s", arquivo);
	file = fopen(arquivo, "r");

	//ATENCAO!!!!
	//ONDE DEIXAR ISSO??????
	//IGNORANDO O P2 DO ARQUIVO
	fscanf(file, "%*c %*d");

	free(arquivo);
	return file;
}

//Funcao para armazenar altura e largura
//O tom maximo de cinza eh ignorado ja que todos os captchas assumem valor 1 como maximo
void imageSize(FILE *file, int *largura, int *altura){
	fscanf(file, "%d %d", largura, altura);
	fscanf(file, "%*d");
}

//Funcao para ler o captcha e armazenar seus valores
int **readImage(FILE *file, int largura, int altura){
	int i, j;
	int **matriz_captcha;

	matriz_captcha = alocMatrix(largura, altura);
	for(i = 0; i < altura; i++){
		for(j = 0; j < largura; j++){
			fscanf(file, "%d", &matriz_captcha[i][j]);
		}
	}
	return matriz_captcha;
}

void bubbleSort(int *valor){
	int tamanho, i, j, aux;
	tamanho = 9;
	for(i = 0; i < tamanho -1; i++){
		for(j = i + 1; j < tamanho; j++){
			if(valor[i] > valor [j]){
				aux = valor[i];
				valor[i] = valor[j];
				valor[j] = aux;
			}
		}
	}
}

void medianFilter(int **matriz_captcha, int largura, int altura){
	int x, y;
	int *valor;
	valor = (int *) malloc(9 * sizeof(int));
	for(x = 1; x < altura - 1; ++x){
		for(y = 1; y < largura - 1; ++y){
			valor[0] = matriz_captcha[x - 1][y - 1];
			valor[1] = matriz_captcha[x - 1][y];
			valor[2] = matriz_captcha[x - 1][y + 1];
			valor[3] = matriz_captcha[x][y - 1];
			valor[4] = matriz_captcha[x][y];
			valor[5] = matriz_captcha[x][y + 1];
			valor[6] = matriz_captcha[x + 1][y - 1];
			valor[7] = matriz_captcha[x + 1][y];
			valor[8] = matriz_captcha[x + 1][y + 1];

			bubbleSort(valor);
			matriz_captcha[x][y] = valor[4];
		}
	}
}
