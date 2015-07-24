/*
 * prototypes.h
 *
 *  Created on: Jan 22, 2015
 *      Authors: Doris, James Vaughan
 */

#ifndef PROTOTYPES_H_
#define PROTOTYPES_H_

#include <pthread.h>

int str_split(char**, char*, char*, int);
int verify_nmea(char *);


// log file
void parse_rmc(char*);
void save_log_value(int, double, int);
void write_log();
void log_error(char *message);
void parseSonar(int index, int value);

// main
void* gpsRead();
void* spRead();
void* sonarRead();
void* imuRead();
void* logWriter();


#endif /* PROTOTYPES_H_ */
