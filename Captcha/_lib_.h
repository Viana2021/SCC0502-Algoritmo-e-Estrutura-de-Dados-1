#ifndef _lib_
#define _lib_

#define SUCCESS 0
#define ALOCATION_ERROR -1

FILE *createFile(char *);
char *alocString();
int **alocMatrix(int, int);
void imageSize(FILE *, int *, int *);
int **readImage(FILE *, int, int);
int freeMatrix(int **, int);
int compareNumber(int **, int, int);
void medianFilter(int **, int, int);

#endif
