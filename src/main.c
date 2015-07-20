/*
 * main.c
 *
 *      Author: James Vaughan
 */

#include <pthread.h>
#include "prototypes.h"

#define MAX_THREADS 2

int main() {
	pthread_t threads[MAX_THREADS];

	/* pthread_create(&threads[0], NULL, gpsRead, NULL); */
	pthread_create(&threads[1], NULL, spRead, NULL);

	int i;
	for (i = 0; i < MAX_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}
