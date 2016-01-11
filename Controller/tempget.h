#ifndef TEMPGET
#define TEMPGET

///////////////////////////////////////////////////////////////////////////////
//			     INCLUDES AND DEFINITIONS			     //
///////////////////////////////////////////////////////////////////////////////

// This program will find all the temperature sensors in the W1 buss,
// extract the folder names, read the w1_slave data file, strip the
// temperature data from it, convert it to a numeric value, compare it
// to a alarm temperature, and display a table with all of the data

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbmsg.h"

// The devices are all located in a standard location by a kerner module
// loaded at boot. The data files all have the same name, but they are in
// thier own folders. The whole path will be:
//				"/sys/bus/w1/device/28-00000545274a/w1_slave"
#define W1_DEVICE_PATH		"/sys/bus/w1/devices/"
#define DATAFILE		"/w1_slave"

// How many sensors are allegedly installed?
#define NUM_SENS		5

///////////////////////////////////////////////////////////////////////////////
//				FUNCTION PROTOYPES			     //
///////////////////////////////////////////////////////////////////////////////

double read_temp(FILE *file_device);
// read_temp::
// 	Takes a w1_slave data file and strips it of all the junk
// 	Returns a double precision numeric value

void find_thermometers();
// find_thermometers::
//	Runs the system commands to grep the thermometer folder paths
//	Updates global string array with folder names

extern void get_temps();

///////////////////////////////////////////////////////////////////////////////
//				GLOBAL VARIABLES			     //
///////////////////////////////////////////////////////////////////////////////

// An array of X folder names, to hold the thermometer data file locations
extern char thmtr[NUM_SENS][16];
extern double temps[NUM_SENS];

///////////////////////////////////////////////////////////////////////////////
//				MAIN PROGRAM				     //
///////////////////////////////////////////////////////////////////////////////
void get_temps () {
	// carve out some space for the temps to be stored
	find_thermometers();
	// Loop through each folder
	int i = 0;
	for ( i = 0; i < NUM_SENS; i++) {
		// catch an empty folder name
		if (!strcmp(thmtr[i], "\0")) {
		} else {
			// Build a file path to the thermometer
			char Thmtr_path[45] = "\0";
			sprintf(Thmtr_path, "%s%s%s", W1_DEVICE_PATH, thmtr[i], DATAFILE);
//			strncpy(Thmtr_path, W1_DEVICE_PATH, 21);
//			strncat(Thmtr_path, thmtr[i], 15);
//			strncat(Thmtr_path, DATAFILE, 9);
			printf("%s:\t", Thmtr_path);
			// open the file using the path we built
			FILE *tdata = fopen(Thmtr_path,"r");
			// catch errors
			if (tdata == 0 ) {
			printf("File Open Error!\n");
			} else {
				temps[i] = read_temp(tdata)/1000;
				printf("%.3f*C\n",temps[i]);
				add_sensor_log(i,(float)temps[i]);
				// close file
				fclose(tdata);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void find_thermometers () {
	// Scrub all the devie folder names from the variable
	int i = 0;
	for ( i = 0; i < NUM_SENS; i++){
		strcpy(thmtr[i],"\0");
	}
	// Grep the device folders from the bus and store the list in a temporary file
	system("ls /sys/bus/w1/devices | grep -e 28-00000******* > /home/pi/cooler/devices");
	// Open the file
	FILE *dev_list = fopen("/home/pi/cooler/devices","r");
	// Catch file opening errors
	if (dev_list == 0) {
	} else {
		// Loop through each line in the file untill EOF, pulling the folder names out
		for ( i = 0; i < NUM_SENS; i++) {
			fgets(thmtr[i], 16, dev_list);
			// Skip the "\n" character by moving 1 byte past the current location
			fseek(dev_list, 1, SEEK_CUR);
//d-bug			printf("%s\n", thmtr[i]);
		}
	}
	// Close the file
	fclose(dev_list);
	// Delete the list, now that we no longer need it
	system("rm /home/pi/cooler/devices");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double read_temp (FILE *file_device) {
	// Placeholder for the line to be read, as well as the length
	char line[60];
	int length;
	// The actual value we want to get from the sensor file
	char extract[5];
	// A junk location needed for the "strtod" function
	char *ptr;
	// Read a line in, and then read the next line.
	fgets(line, 60, file_device);
	fgets(line, 60, file_device);
	// Find the location of the last value in the line
	length = strlen(line);
	// Shift left 5 spaces (6 including the null)
	length -= 6;
	// Copy the last 5 characters into the string you're capturing
	strncpy(extract,&line[length],5);
	// Return the value to the caller
	return strtod(extract,&ptr);
}

#endif
