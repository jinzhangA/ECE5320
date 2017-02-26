#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>	/* for uint64 definition */
#include <time.h>	/* for clock_gettime */
#include <math.h>
#include <pthread.h>
#include <math.h>

//#define N 1000
#define SEED 2
#define BILLION 1000000000L
#define MAX_NUM_THRDS 8
void **generateMat();
void printMat(int **mat);
int way1Sort(int **mat);
void maxAndSwap(int **mat, int col);
void *paraMax(void *arg);

int maxVal = 0;
int maxRow = 0;
int **mat;
int N;
int power;
uint64_t timer = 0;


pthread_t thr_id[MAX_NUM_THRDS];
pthread_mutex_t maxLock;
pthread_mutex_t maxRowLock;

struct ptArg{
	int start;
	int col;
	int size;
};

struct ptArg thread_data_array[MAX_NUM_THRDS];

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
	
//	uint64_t diff;
//	struct timespec start, end;
//	clock_gettime(CLOCK_MONOTONIC, &start);	
	for (i = 0; i < N; i++){
		maxAndSwap(mat, i);
	}
//	clock_gettime(CLOCK_MONOTONIC, &end);
//	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	printf("For 2^%d size, time = %llu nanoseconds\n", power, (long long unsigned int) timer);
	return 0;
}

void maxAndSwap(int **mat, int col){
//	struct timespec start, finish;
	uint64_t diff;
	struct timespec start, end;
	int rc, ntime, stime;
	int i;
	int j;
	int stepSize;
	int colLength = N - col;
	int numThreads;
	void *status;
	pthread_attr_t attr;

	pthread_mutex_init (&maxLock, NULL);
	pthread_mutex_init (&maxRowLock, NULL);
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);
	clock_gettime(CLOCK_REALTIME, &start);

	maxVal = mat[col][col];
	maxRow = col;
	
	
	if (colLength > MAX_NUM_THRDS * 2){
		numThreads = MAX_NUM_THRDS;
		stepSize = colLength/numThreads;
	}else{
		numThreads = colLength/2;
//		printf("numThreads%d col%d\n", numThreads, col);
		stepSize = 2;
	}
	
	for (i = 0; i < numThreads - 1; i++){
		thread_data_array[i].start = col + i*stepSize;
		thread_data_array[i].size = stepSize;
		thread_data_array[i].col = col;
	}
	
	thread_data_array[numThreads - 1].start = col + (numThreads - 1)*stepSize;
	thread_data_array[numThreads - 1].size = colLength - (numThreads - 1)*stepSize;
	thread_data_array[numThreads - 1].col = col;
	

	clock_gettime(CLOCK_MONOTONIC, &start);	
	
	for (i = 0; i < numThreads; i++){
		rc = pthread_create(&thr_id[i], &attr, paraMax, (void *)&thread_data_array[i]);
	}
	pthread_attr_destroy(&attr);
	
	for (i = 0; i < numThreads; i++){
		pthread_join(thr_id[i], &status);
	}
	
	clock_gettime(CLOCK_MONOTONIC, &end);
	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	timer += diff;
	pthread_mutex_destroy(&maxLock);
	pthread_mutex_destroy(&maxRowLock);
//	printf("max is: %d row %d\n", maxVal, maxRow);
	
	int *temp = mat[col];
	mat[col] = mat[maxRow];
	mat[maxRow] = temp;

//	printMat(mat);
	
//	clock_gettime(CLOCK_REALTIME, &finish);
//	ntime = finish.tv_nsec - start.tv_nsec;
//	stime = (int) finish.tv_nsec - (int) start.tv_nsec;
//	printf ("%d", stime);
	
//	pthread_exit(NULL);
}

void *paraMax(void *arg){
	struct ptArg *myArg;
	
	myArg = (struct ptArg *) arg;
	int start = myArg->start;
	int col = myArg->col;
	int size = myArg->size;
	int i;
	int localMax = mat[start][col];
	int localRowMax = myArg->start;
	
	
//	printf("start:%d col:%d size:%d\n", start, col, size);
	for (i = start; i < start + size; i++){
		if (abs(mat[i][col]) > localMax){
			localMax = abs(mat[i][col]);
			localRowMax = i;
		}
	}
//	printf("Local Max value: %d, row: %d\n", localMax, localRowMax);
	pthread_mutex_lock (&maxLock);
	pthread_mutex_lock (&maxRowLock);
	if (localMax > maxVal){
		maxVal = localMax;
		maxRow = localRowMax;
	}
	pthread_mutex_unlock (&maxLock);
	pthread_mutex_unlock (&maxRowLock);
	
	pthread_exit(NULL);
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
	fp=fopen("way1Para.csv","w+");
	for (power = 4; power < 15; power++){
		N = pow(2, power);
		generateMat();
		way1Sort(mat);
		free(mat);
		fprintf(fp, "%d, %llu\n", s, (long long unsigned int) timer);
	}
	fclose(fp);
	
	exit(0);
}