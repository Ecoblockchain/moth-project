/*
 * main.c
 *
 *      Author: James Vaughan
 */

#include <pthread.h>
#include "prototypes.h"

#define MAX_THREADS 3

int main() {
	pthread_t threads[MAX_THREADS];
	int tid[MAX_THREADS];

	tid[0] = pthread_create(&threads[0], NULL, gpsRead, NULL);
	/* tid[1]= pthread_create(&threads[1] NULL, spRead, NULL); */
	tid[2] = pthread_create(&threads[2], NULL, statusLED, NULL);

	for (int i = 0; i < MAX_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}
