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

	return 0;
}
