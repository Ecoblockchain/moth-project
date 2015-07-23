/*
 * prototypes.h
 *
 *  Created on: Jan 22, 2015
 *      Authors: Doris, James Vaughan
 */

#ifndef PROTOTYPES_H_
#define PROTOTYPES_H_
int create_nmea(char*);
void init_tty();
void sendLine(char*);
void sendNmea(char*);
double get_heel();
int str_split(char**, char*, char*, int);
void enter_test_mode(char**);
void exit_text_mode();
void init_cal_array_defaults();
void save_cal_array(char*);
void get_cal_string(char*, int);
void read_cal_array(char*);
void write_to_cal_array(int, double);
void save_timestamp(char*);
double read_from_cal_array(int);
int str_split(char **, char *, char *, int );
void load_cal_name();
void nmea_write(char*);
int verify_nmea(char *);

// log file
void parse_rmb(char*);
void parse_rmc(char*);
void save_log_value(int, double);
void write_log();
void log_error(char *message);
void parseSonar(int index, int value);

// main
void * gpsRead();
void * spRead();
void * statusLED();


#endif /* PROTOTYPES_H_ */
