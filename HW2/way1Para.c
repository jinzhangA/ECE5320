// Zhuo Chen zc292
// compile with: gcc way1Para.c -o way1Para -lpthread -lm

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>	/* for uint64 definition */
#include <time.h>	/* for clock_gettime */
#include <math.h>
#include <pthread.h>
#include <math.h>

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

// generate the matrix
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
	
	for (i = 0; i < N; i++){
		maxAndSwap(mat, i);
	}
	
	return 0;
}

void maxAndSwap(int **mat, int col){
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
	
	// initialize the malVal and row each time running;
	maxVal = mat[col][col];
	maxRow = col;
	
	// Determine the number of treads. Will be explained in detail
		// in the code write up
	if (colLength > MAX_NUM_THRDS * 2){
		numThreads = MAX_NUM_THRDS;
		stepSize = colLength/numThreads;
	}else{
		numThreads = colLength/2;
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
	
	// get the start time
	clock_gettime(CLOCK_MONOTONIC, &start);	
	// creat the threads
	for (i = 0; i < numThreads; i++){
		rc = pthread_create(&thr_id[i], &attr, paraMax, (void *)&thread_data_array[i]);
	}
	pthread_attr_destroy(&attr);
	
	// join the threads to run them
	for (i = 0; i < numThreads; i++){
		pthread_join(thr_id[i], &status);
	}
	// get the start time
	clock_gettime(CLOCK_MONOTONIC, &end);
	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	timer += diff;
	pthread_mutex_destroy(&maxLock);
	pthread_mutex_destroy(&maxRowLock);
	
	// after all threads, swap the diagonal row with the maximum value row.
	int *temp = mat[col];
	mat[col] = mat[maxRow];
	mat[maxRow] = temp;
}

// pthread method for way1
void *paraMax(void *arg){
	struct ptArg *myArg;
	
	// extract the arguments from the structure 
	myArg = (struct ptArg *) arg;
	int start = myArg->start;
	int col = myArg->col;
	int size = myArg->size;
	int i;
	
	// initialize the local maximum with the first element
	int localMax = mat[start][col];
	int localRowMax = myArg->start;
	
	// if the element is larger than the local maximum, update the local maximum 
	for (i = start; i < start + size; i++){
		if (abs(mat[i][col]) > localMax){
			localMax = abs(mat[i][col]);
			localRowMax = i;
		}
	}
	
	// after we find the local maximum, compare it with the global maximum;
	// the global maximum need to be locked for both read and write.
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
	// code for extracting timing information into csv file
	FILE *fp;
	fp=fopen("way1Para.csv","w+");
	
	// looping over different size of matrix and benmark
	for (power = 4; power < 12; power++){
		N = pow(2, power);
		timer = 0;
		generateMat();
		way1Sort(mat);
		free(mat);
		printf("For 2^%d size, time = %llu nanoseconds\n", power, (long long unsigned int) timer);
		fprintf(fp, "%d, %llu\n", power, (long long unsigned int) timer);
	}
	fclose(fp);
	
	exit(0);
}