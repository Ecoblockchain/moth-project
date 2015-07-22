/*
 * main.c
 *
 *      Author: James Vaughan
 */

#include <pthread.h>
#include "prototypes.h"

#define MAX_THREADS 2

int main() {
	pthread_t thread;

	pthread_create(&thread, NULL, gpsRead, NULL);
	pthread_join(thread, NULL);

	/* int i; */
	/* for (i = 0; i < MAX_THREADS; i++) { */
	/* 	pthread_join(threads[i], NULL); */
	/* } */
	return 0;
}
