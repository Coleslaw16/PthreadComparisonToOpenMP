//Timer Comparison of OpenMP to Pthreads
//By John Stone

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cstdlib>		
#include <pthread.h>	
#include "chTimer.h"
#include <omp.h>

#define NUM_THREADS 8
#define _N 80000000	

//Allocate vectors for addition
float *a = (float *)malloc(_N * sizeof(float));
float *b = (float *)malloc(_N * sizeof(float));
float *c = (float *)malloc(_N * sizeof(float));

//Method which performs vector addition for pthreads
void *pthreadVectorAdd(void *t)
{
	long i;
	long tid = (long)t;

	for (i = tid; i < _N; i += NUM_THREADS)
	{
		c[i] = a[i] + b[i];
	}

	return NULL;
}

void setupPThread()
{
	int rc;
	long t;
	void *status;
	pthread_t thread[NUM_THREADS];
	chTimerTimestamp start, stop;
	
	chTimerGetTime(&start);
	for (t = 0; t < NUM_THREADS; t++)
	{
		rc = pthread_create(&thread[t], NULL, pthreadVectorAdd, (void *)t);
		if (rc)
		{
			printf("Error");
			exit(-1);
		}
	}

	for (t = 0; t < NUM_THREADS; t++)
	{
		rc = pthread_join(thread[t], &status);
	}
	chTimerGetTime(&stop);
	printf("The time it took for pthread was %f\n", chTimerElapsedTime(&start, &stop));
}

//Open MP vector add
void openMPVectorAdd()
{

#pragma omp parallel for num_threads(8)
	for (int i = 0; i < _N; i++)
	{
		c[i] = a[i] + b[i];
	}
}

void openMPSetup()
{
	chTimerTimestamp start, stop;
	chTimerGetTime(&start);
	openMPVectorAdd();
	chTimerGetTime(&stop);
	printf("The time it took for openMP was %f\n", chTimerElapsedTime(&start, &stop));
}

int main(int argc, char *argv[])
{
/*	float *a, *b, *c;
	a = (float *)malloc(_N * sizeof(float));
	b = (float *)malloc(_N * sizeof(float));	
	c = (float *)malloc(_N * sizeof(float)); */

	for (int i = 0; i < _N; i++)
	{
		a[i] = rand();
		b[i] = rand();
		c[i] = 0;
	}

	openMPSetup();
	for (int i = 0; i < _N; i++)
	{
		a[i] = rand();
		b[i] = rand();
		c[i] = 0;
	}
	setupPThread();

	getchar();
}



