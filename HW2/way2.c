#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>	/* for uint64 definition */
//#include <omp.h>
#include <time.h>	/* for clock_gettime */

#define N 4
#define SEED 2
#define BILLION 1000000000L

int **mat;

void **generateMat();
void printMat(int **mat);
void way2Sort();
void mergeSort(int *value, int *index, int n);
void merge(int *values, int *valueL, int *valueR, int *indexs, int *indexL, int *indexR, int leftCount, int rightCount);

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

void way2Sort(){
	int i;
	int j;
	int size;
	int *values, *indexs, **indexCache;
	
	uint64_t diff;
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);	
	 
	for (i = 0; i < N - 1; i++){
		size = N - i;
		values = (int*)malloc(size*sizeof(int));
		indexs = (int*)malloc(size*sizeof(int));
		for (j = 0; j < size; j++){
			indexs[j] = j + i;
			values[j] = mat[j + i][i];
//			printf("%d ", values[j]);
		}
//		printf("\n");
		mergeSort(values, indexs, N - i);
		for (j = 0; j < size; j++){
			printf("%d ", indexs[j]);
		}
		printf("\n");
		indexCache = (int**)malloc(size*sizeof(int *));
		for (j = 0; j < size; j++){
			indexCache[j] = mat[j + i];
		}
		for (j = 0; j < size; j++){
			printf("cache: %d ", indexCache[j]);
		}
		printf("\n");
		
		for (j = i; j < size + i; j++){
			printf("j: %d\n", j);
			mat[j] = indexCache[indexs[j - i]];
		}
		free(values);
		free(indexs);
		free(indexCache);
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) diff);
	printMat(mat);
}

void mergeSort(int *values, int *indexs, int n){
	int mid, i, *valueL, *valueR, *indexL, *indexR;
	if (n < 2) return;
	mid = n/2;
	
	valueL = (int *)malloc(mid*sizeof(int));
	valueR = (int *)malloc((n - mid)*sizeof(int));
	
	indexL = (int *)malloc(mid*sizeof(int));
	indexR = (int *)malloc((n - mid)*sizeof(int));
	for (i = 0; i < mid; i++){
		valueL[i] = values[i];
		indexL[i] = indexs[i];
	}
	
	for (i = mid; i < n; i++){
		valueR[i - mid] = values[i];
		indexR[i - mid] = indexs[i];
	}
	
	mergeSort(valueL, indexL, mid);
	mergeSort(valueR, indexR, n - mid);
	merge(values, valueL, valueR, indexs, indexL, indexR, mid, n - mid);
	free(valueL);
	free(valueR);
	free(indexL);
	free(indexR);
	
}

void merge(int *values, int *valueL, int *valueR, int *indexs, int *indexL, int *indexR, int leftCount, int rightCount){
	int i, j, k;
	
	i = 0;
	j = 0;
	k = 0;
	
	while (i < leftCount && j < rightCount){
		if (valueL[i] > valueR[j]){
			values[k] = valueL[i];
			indexs[k] = indexL[i];
			k++;
			i++;
		}else{
			values[k] = valueR[j];
			indexs[k] = indexR[j];
			k++;
			j++;
		}
	}
	
	while(i < leftCount){
		values[k] = valueL[i];
		indexs[k] = indexL[i];
		k++;
		i++;
	}
	
	while(j < rightCount){
		values[k] = valueR[j];
		indexs[k] = indexR[j];
		k++;
		j++;
	}
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
	generateMat();
	printMat(mat);
	printf("\n");
	way2Sort();
	free(mat);
	exit(0);
}