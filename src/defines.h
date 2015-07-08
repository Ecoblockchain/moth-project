/*
 * defines.h
 *
 *  Created on: Dec 19, 2014
 *      Author: Doris
 */

#ifndef DEFINES_H_
#define DEFINES_H_
#define ADDON 1
#define MRAA_SUCCESS 0
#define MRAA_ERROR_PLATFORM_ALREADY_INITIALISED 12

//leeway
#define		leeway_factor	0
#define		target_heel		1
#define		leeway_clip		2

//compass
#define		hdg_correction		3
#define		ns_correction		4
#define		ew_correction		5

//boat speed
#define		bs_gain				6
#define		bs_offset 	    	7
#define		port_m		        8
#define		starboard_m		    9


//wind direction
#define		upwash_m		10  // this is usually between .2 and .35.  .275 is default but only set after upwash_b is set
#define		upwash_b		11
#define		upwash_down		12


//wind speed
#define		aws_correction	13 // knots of aws correction
#define		aws_ref_tws		14 // knots reference tws

//status
#define		hdg_on			15
#define		bs_on			16
#define		aws_on			17
#define		awa_on			18
#define		leeway_on		19
#define		twd_on			20
#define		tws_on			21
#define		gps_to_nmea		22

//true wind direction (tweaks)
#define		twd_correction_up		23
#define		twd_correction_reach	24
#define		twd_correction_down		25
#define		twd_correction_speed	26

// settings
#define		sail_reef				27

// heel sencor
#define		use_heel_sensor			28
#define		heel_correction_m		29
#define		heel_correction_b		30

#define		stowe_display			31
#define		variation				32

// wind sensor
#define		wind_input_offset		33
#define		wind_sinx				34
#define		wind_cosx				35
#define		wind_sin2x				36
#define		wind_cos2x				37
#define		wind_output_offset		38

// tacktick tack to tack offset
#define		port_m2		        	39
#define		starboard_m2		    40

// number of corrections
#define		cal_array_max			41

#define STOWE // compiler option







#endif /* DEFINES_H_ */

