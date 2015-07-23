/*
 * main.c
 *
 *      Author: James Vaughan
 */

#include <pthread.h>
#include "prototypes.h"

#define MAX_THREADS 2

int main() {
	pthread_t gpsThread;
	pthread_t sonarThread;

	pthread_create(&gpsThread, NULL, gpsRead, NULL);
	pthread_create(&sonarThread, NULL, gpsRead, NULL);
	pthread_join(gpsThread, NULL);
	pthread_join(sonarThread, NULL);
	
	return 0;
}
