#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>	/* for uint64 definition */
#include <time.h>	/* for clock_gettime */
#include <math.h>
#include <pthread.h>

#define N 4
#define SEED 2
#define BILLION 1000000000L
#define NUM_THRDS 1
int **generateMat();
void printMat(int **mat);
int way1Sort(int **mat);
void maxAndSwap(int **mat);
void *paraMax(void *arg);

int maxVal = 0;
int **mat;

pthread_t thr_id[NUM_THRDS];
pthread_mutex_t maxLock;


struct ptArg{
	int start;
	int col;
};

struct ptArg thread_data_array[NUM_THRDS];
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

void maxAndSwap(int **mat){
	struct timespec start, finish;
	int rc, ntime, stime;
	long i;
	long j;
	void *status;
	pthread_attr_t attr;

	pthread_mutex_init (&maxLock, NULL);
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);
	clock_gettime(CLOCK_REALTIME, &start);
	
	for (i = 0; i < NUM_THRDS; i++){
		rc = pthread_create(&thr_id[i], &attr, paraMax, (void *)&thread_data_array[i]);
	}
	pthread_attr_destroy(&attr);
	
	for (i = 0; i < NUM_THRDS; i++){
		pthread_join(thr_id[i], &status);
	}
	
	clock_gettime(CLOCK_REALTIME, &finish);
	ntime = finish.tv_nsec - start.tv_nsec;
	stime = (int) finish.tv_nsec - (int) start.tv_nsec;
	printf ("%d", stime);
	pthread_mutex_destroy(&maxLock);
	pthread_exit(NULL);
}

void *paraMax(void *arg){
	struct ptArg *myArg;
	
	myArg = (struct ptArg *) arg;
	int start = myArg->start;
	int col = myArg->col;

	int i;
	int localMax = 0;
	
	for (i = start; i < N; i++){
		if (abs(mat[i][col]) > localMax){
			localMax = abs(mat[i][col]);
		}
	}
	if (localMax > maxVal){
		pthread_mutex_lock (&maxLock);
		maxVal = localMax;
		pthread_mutex_unlock (&maxLock);
	}
	
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
	mat = generateMat();
	printMat(mat);
	printf("\n");
	maxAndSwap(mat);
	printMat(mat);
	free(mat);
	exit(0);
}