/*
 * prototypes.h
 *
 *  Created on: Jan 22, 2015
 *      Authors: Doris, James Vaughan
 */

#ifndef PROTOTYPES_H_
#define PROTOTYPES_H_
int create_nmea(char*);
int gx_display(int display, char*, char*, char*, char*, char*, char*);
int gx_display_new(int, int, char*,char*, char*, char*, char*, char*, char*, char*, char*, char*, char*, char*);
int read_stowe_key_code();
void init_tty();
void sendLine(char*);
void sendNmea(char*);
double get_heel();
int init_heel();
int str_split(char**, char*, char*, int);
int init_bluetooth();
int ping_bluetooth();
int restore_bluetooth();
int write_bluetooth(char*);
void process_blue(char*);
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
void display_stowe_3(char*);
void send_data_to_bluetooth(int);
int verify_nmea(char *);

// log file
void parse_rmb(char*);
void parse_rmc(char*);
void save_log_value(int, double);
void write_log();

// current related
double normalize(double);
void get_current (double, double, double*);

void write_lcd(char*, int, int, char*, int, int);
int init_lcd();

void send_debug_data_to_bluetooth();

void vector_add(double*, double*, double*);
void vector_sub(double*, double*, double*);


// James
void * gpsRead();
void * spRead();
void * statusLED();


#endif /* PROTOTYPES_H_ */
