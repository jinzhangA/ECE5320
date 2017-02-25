#include <stdio.h>
#include <stdlib.h>

//#include <>
#define N 4
#define SEED 2

int **generateMat();
void printMat(int **mat);
int way2Sort(int **mat);
void bubble_srt_vertical( int **mat, int start, int len);

int **generateMat() {
	int i, j;
	int **mat;
	
	mat = (int **)malloc(N * sizeof(int*));
	srand(SEED);
	
	for (i = 0; i < N; i++){
		mat[i] = (int *)malloc(N * sizeof(int));
		for (j = 0; j < N; j++){
			mat[i][j] = rand()%1000;
		}
	}
	
	return mat;
}

int way2Sort(int **mat){
	int i;
	int j;
	
	for (i = 0; i < N; i++){
//		printf("Sorting NO.%d\n", i);
		bubble_srt_vertical(mat, i, (N - i));
//		printf("\n");
	}
	printMat(mat);
	return 0;
}

void printMat(int **mat){
	int i;
	int j;
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}
}

void bubble_srt_vertical( int **mat, int start, int len)  
{   
	int i, j;
//	printf("lalala: %d %d\n", start, len);
	for(i = 0; i < len - 1; i++)	{              
		for(j = 0; j < len - i - 1; j++) {
//			printf("going: %d %d\n", i, j);   
		   if(mat[j + start][start] < mat[j + 1 + start][start]){
//			printf("i=%d Swap %d, %d\n", i, j+ start, j + 1+ start);
			int *temp = mat[j + start];
			mat[j + start] = mat[j + 1 + start];
			mat[j + 1 + start] = temp;
//			printMat(mat);
//			printf("\n");
			}
		}
	}
}



int main(int argc, char **argv){
	int **mat;
	mat = generateMat();
	printMat(mat);
	printf("\n");
	way2Sort(mat);
	exit(0);
}