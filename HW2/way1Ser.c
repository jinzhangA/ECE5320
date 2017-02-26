#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>	/* for uint64 definition */
#include <time.h>	/* for clock_gettime */
#include <math.h>

#define SEED 2
#define BILLION 1000000000L

void **generateMat();
void printMat(int **mat);
int way1Sort(int **mat);
void maxAndSwap(int **mat, int col);
int **mat;
int power;
uint64_t timer;
int N;

void **generateMat() {
	int i, j;
	mat = (int **)malloc(N * sizeof(int*));
	srand(SEED);
	
	for (i = 0; i < N; i++){
		mat[i] = (int *)malloc(N * sizeof(int));
		for (j = 0; j < N; j++){
			mat[i][j] = rand()%1000;
		}
	}
}

int way1Sort(int **mat){
	int i;
	int j;
	
	uint64_t diff;
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);	
	for (i = 0; i < N; i++){
		maxAndSwap(mat, i);
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	timer = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	return 0;
}

void maxAndSwap(int **mat, int col){
	int i;
	int j;
	int max = abs(mat[col][col]);
	int maxI = col;
	for (i = col; i < N; i++){
		for (j = col; j < N; j++){
			if (abs(mat[i][j]) > max){
				maxI = i;
			}
		}
	}
	
	int *temp = mat[col];
	mat[col] = mat[maxI];
	mat[maxI] = temp;
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

int main(int argc, char **argv){
	FILE *fp;
	fp=fopen("way1Ser.csv","w+");
	for (power = 4; power < 12; power++){
		timer = 0;
		N = pow(2, power);
		generateMat();
		way1Sort(mat);
		free(mat);
		printf("For 2^%d size, time = %llu nanoseconds\n", power, (long long unsigned int) timer);
		fprintf(fp, "%d, %llu\n", power, (long long unsigned int) timer);
	}
	fclose(fp);
	exit(0);
}