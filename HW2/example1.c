#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void OddEvenSort( int *A, int N);

void OddEvenSort( int *A, int N) {
	int exch = 1, start = 0, i; 
	int temp;
	while (exch || start) { 
		exch = 0;
#pragma omp parallel for private(temp) 
	for (i = start; i < N−1; i += 2) {
		if (A[i]>A[i+1]){
			temp = A[i]; A[i] = A[i+1]; 
			A[i+1] = temp; exch = 1;
		} 
	}
	if (start == 0) start = 1; 
	else start = 0;
	}
}

int main(int argc, char **argv){
	int i = 0;
	int array[5] = {1, 3, 7, 2, 4};
	OddEvenSort(&array, 5);
	for (i = 0; i < 5; i ++){
		printf(array[i]);
	}
}
