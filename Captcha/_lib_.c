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
FILE *createFile(char *arquivo){
	FILE *file;
	if(!arquivo)
		return (FILE *) ALOCATION_ERROR;

	file = fopen(arquivo, "r");

	//Ignorando o P2 do arquivo, ja que so serao tratados arquivos do tipo P2
	fscanf(file, "%*c %*d");

	return file;
}

//Funcao para armazenar altura e largura
//O tom maximo de cinza eh ignorado ja que eh irrelevante para esse projeto
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

//Funcao para execucao de um bubble sort para ordenacao
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

//Aplicacao do median filter, alterando diretamente a matriz de captcha ja que o resultado final devem ser apenas numeros
void medianFilter(int **matriz_captcha, int largura, int altura){
	int x, y;
	int *valor;
	valor = (int *) malloc(9 * sizeof(int));
	for(x = 1; x < altura - 1; x++){
		for(y = 1; y < largura - 1; y++){
			valor[0] = matriz_captcha[x - 1][y - 1];
			valor[1] = matriz_captcha[x][y - 1];
			valor[2] = matriz_captcha[x + 1][y - 1];
			valor[3] = matriz_captcha[x - 1][y];
			valor[4] = matriz_captcha[x][y];
			valor[5] = matriz_captcha[x + 1][y];
			valor[6] = matriz_captcha[x - 1][y + 1];
			valor[7] = matriz_captcha[x][y + 1];
			valor[8] = matriz_captcha[x + 1][y + 1];

			bubbleSort(valor);
			matriz_captcha[x][y] = valor[4];
		}
	}
}

void compareMask(int **matriz_captcha, int x, int y){
	FILE **file;
	file = (FILE **) malloc(10 * sizeof(FILE *));
	int largura, altura;
	int cont = 0;
	int **matriz_temp;
	int i, j, k, l;
	int *cont_igual;
	cont_igual = (int *) calloc(10, sizeof(int));

	file[0] = createFile("./mascaras/0.pgm");
	file[1] = createFile("./mascaras/1.pgm");
	file[2] = createFile("./mascaras/2.pgm");
	file[3] = createFile("./mascaras/3.pgm");
	file[4] = createFile("./mascaras/4.pgm");
	file[5] = createFile("./mascaras/5.pgm");
	file[6] = createFile("./mascaras/6.pgm");
	file[7] = createFile("./mascaras/7.pgm");
	file[8] = createFile("./mascaras/8.pgm");
	file[9] = createFile("./mascaras/9.pgm");

	//percorrendo ponteiros para que ja apontem para o primeiro pixel das mascaras
	for(i = 0; i < 10; i++){
		imageSize(file[i], &largura, &altura);
	}

	while(cont < 10){
		if(cont == 1){
			matriz_temp = readImage(file[cont], largura, altura);
			for(i = 0; i < 50; i++){
				for(j = 0; j < 20; j++){
					if(matriz_temp[i][j] == 0)
						matriz_temp[i][j] = 1;
					else
						matriz_temp[i][j] = 0;
				}
			}
		}
		else
			matriz_temp = readImage(file[cont], largura, altura);
		for(i = x, k = 0; i < x + 50; i++, k++){
			for(j = y, l = 0; j < y + 30; j++, l++){
				if(matriz_captcha[i][j] == matriz_temp[k][l])
					cont_igual[cont] += 1;
			}
		}
		cont++;
	}

	int maior = 0;
	for(i = 0; i < 10; i++){
		if(cont_igual[maior] < cont_igual[i])
			maior = i;
	}
	printf("%d", maior);
}

//Funcao para comparacao do resultado do captcha com as mascaras
int compareNumber(int **matriz_captcha, int largura_captcha, int altura_captcha){

	int x, y;
	int cont1 = 0;

	for(y = 1; y < largura_captcha; y++){
		cont1 = 0;
		for(x = 0; x < altura_captcha; x++){
			if(cont1 == 5){
				compareMask(matriz_captcha, x - 7, y);
				x = altura_captcha;
				y += 30;
				cont1 = 0;
			}
			else{ 
				if(matriz_captcha[x][y] == 1)
					cont1++;
				else
					cont1 = 0;
			}
		}
	}
	printf("\n");
	return SUCCESS;
}
