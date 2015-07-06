#include <pthread.h>

void * gpsRead();

int main() {
	pthread_t gpsThread;
	int gpsTID = pthread_create(&gpsThread, NULL, gpsRead, NULL);
	pthread_join(gpsThread, NULL);

	return 0;
}
