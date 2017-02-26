#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>	/* for uint64 definition */
#include <omp.h>
#include <time.h>	/* for clock_gettime */

//#define N 4
#define SEED 2
#define BILLION 1000000000L

int **mat;
int **tempMat;
uint64_t timer;

void **generateMat();
void printMat(int **mat);
void way2Sort();
void copyMat();
void creatTempMat();
void mergeSort(int *value, int *index, int n);
void mergeSortPara(int *values, int *indexs, int n);
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
	int j, k;
	int size;
	int *values, *indexs, **indexCache;
	
	uint64_t diff;
	struct timespec start, end;
	
	 
	for (i = 0; i < N - 1; i++){
		size = N - i;
		values = (int*)malloc(size*sizeof(int));
		indexs = (int*)malloc(size*sizeof(int));
		for (j = 0; j < size; j++){
			indexs[j] = j + i;
			values[j] = mat[j + i][i];
		}
		clock_gettime(CLOCK_MONOTONIC, &start);	
		mergeSortPara(values, indexs, N - i);
		clock_gettime(CLOCK_MONOTONIC, &end);
		diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
		timer += diff;
		for (j = 0; j < N; j++){
			for (k = 0; k < N; k++){
				tempMat[j][k] = mat[j][k];
			}
		}
	

		for (j = 0; j < size; j++){
			for (k = 0; k < N; k++){
				mat[j + i][k] = tempMat[indexs[j]][k];
			}
			
		}
		printf("\n");
		free(values);
		free(indexs);
	}
	
	
//	printMat(mat);
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

void mergeSortPara(int *values, int *indexs, int n){
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
	#pragma omp parallel sections
	{
		#pragma omp section
		mergeSortPara(valueL, indexL, mid);
		#pragma omp section
		mergeSortPara(valueR, indexR, n - mid);
		
	}
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

void creatTempMat(){
	int i, j;
	tempMat = (int **)malloc(N * sizeof(int*));
	for (i = 0; i < N; i++){
		tempMat[i] = (int *)malloc(N * sizeof(int));
	}
}

void copyMat(){
	int i;
	int j;
	
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			tempMat[i][j] = mat[i][j];
		}
	}
}


int main(int argc, char **argv){
	for (power = 4; power < 12; power++){
		timer = 0;
		N = pow(2, power);
		generateMat();
		creatTempMat();
		copyMat();
		way2Sort();
		free(mat);
		printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) timer);
	}
	exit(0);
}