#include <stdio.h>	/* for printf */
#include <stdint.h>	/* for uint64 definition */
#include <stdlib.h>	/* for exit() definition */
#include <time.h>	/* for clock_gettime */

#define BILLION 1000000000L
#define MIN 1024
#define MAX 67108864
#define K 10

void pb2(){
	FILE *fp;
	fp=fopen("record.csv","w+");
	double diff;
	struct timespec start, end;
	int n, s, i, k;
	
	float temp;
	float *x = (float *)(malloc(MAX*sizeof(float)));

	for (i = 0; i <= MAX; i++){
		x[i] = ((float)rand()/(float)(RAND_MAX));
	}
	for (n = MIN; n <= MAX; n = n*2){
		for (s = 1; s <= n/2; s = s*2){
			clock_gettime(CLOCK_MONOTONIC, &start);	/* mark start time */
			for (k = 0; k < s*K; k++){
				for (i = 0; i < n; i = i + s){
					temp = x[i];
				}
			}
			clock_gettime(CLOCK_MONOTONIC, &end);	/* mark the end time */
			diff = (double) BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
			diff = diff/(n*K);
			printf("%d %lf\n", s, diff);
			fprintf(fp, "%d,%0.2lf\n", s, diff);

		}
	}
	fclose(fp);
}



int main(int argc, char **argv){
	pb2();
	exit(0);
}
