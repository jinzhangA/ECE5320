#include <stdio.h>	/* for printf */
#include <stdint.h>	/* for uint64 definition */
#include <stdlib.h>	/* for exit() definition */
#include <time.h>	/* for clock_gettime */

#define BILLION 1000000000L
#define N 1048576
#define K 64

uint64_t centroid1(int k);
uint64_t centroid2(int k);

//(long long unsigned int) centroid2(int k);

uint64_t centroid1(int k){
	uint64_t diff;
	struct timespec start, end;
	int i = 0;
	int j = 0;
	
	float *x = (float *)(malloc(k*N*sizeof(float)));
	float *c = (float *)(malloc(k*sizeof(float)));

	for (i = 0; i < N*k; i++){
		x[i] = ((float)rand()/(float)(RAND_MAX));
	}
	
	for (i = 0; i < k; i++){
		c[i] = 0;
	}
	
	
	clock_gettime(CLOCK_MONOTONIC, &start);	/* mark start time */
	for (i = 0; i < N; i++){
		for (j = 0; j < k; j++){
			c[j] = c[j] + x[i*k + j];
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &end);	/* mark the end time */
	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	free(x);
	free(c);
	return diff;
}

uint64_t centroid2(int k){
	uint64_t diff;
	struct timespec start, end;
	int i = 0;
	int j = 0;
	
	float *x = (float *)(malloc(k*N*sizeof(float)));
	float *c = (float *)(malloc(k*sizeof(float)));

	for (i = 0; i < N*k; i++){
		x[i] = ((float)rand()/(float)(RAND_MAX));
	}
	
	for (i = 0; i < k; i++){
		c[i] = 0;
	}
	
	
	clock_gettime(CLOCK_MONOTONIC, &start);	/* mark start time */
	for (j = 0; j < k; j++){
		for (i = 0; i < N; i++){
			c[j] = c[j] + x[i*k + j];
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &end);	/* mark the end time */
	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	free(x);
	free(c);
	return diff;
}


int main(int argc, char **argv){
	uint64_t diff;
	struct timespec start, end;
	int i;
	
	long sum = 0;
	float avg;
	for (i = 0; i < 10; i++){
		diff = centroid1(K);;
//		printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);
		sum += diff;
	}
	avg = sum/10.0;
	printf("function 1 average elapsed time = %f nanoseconds\n", avg);
	sum = 0;
	for (i = 0; i < 10; i++){
		diff = centroid2(K);;
//		printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);
		sum += diff;
	}
	avg = sum/10.0;
	printf("function 2 average elapsed time = %f nanoseconds\n", avg);
	
	exit(0);
}
