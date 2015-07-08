/*
 * main.c
 *
 *      Author: James Vaughan
 */

#include <pthread.h>

void * gpsRead();
void * spRead();

int main() {
	pthread_t gpsThread, spThread;
	int gpsTID = pthread_create(&gpsThread, NULL, gpsRead, NULL);
	int spTID = pthread_create(&spThread, NULL, spRead, NULL);
	pthread_join(gpsThread, NULL);
	pthread_join(spThread, NULL);

	return 0;
}
