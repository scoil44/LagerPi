/* TODO:
	Maybe split the control logic and utility funcs into seperate files?
*/


///////////////////////////////////////////////////////////////////////////////
//			 INCLUDES AND CONSTANTS				     //
///////////////////////////////////////////////////////////////////////////////

// This program will monitor the temperature gathered by the sensors and output
// a control signal to the heating/cooling elements.
// It will also relflect these changes in the database.

#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include "tempget.h"
#include "dbmsg.h"

// The following definitions assign quick device names to a GPIO pin
#define HEAT1			RPI_GPIO_P1_12
#define HEAT2			RPI_GPIO_P1_16
#define HEAT3			RPI_GPIO_P1_18
#define HEAT4			RPI_GPIO_P1_22
#define FANS			RPI_GPIO_P1_11
static int heats[5] = {HEAT1, HEAT2, HEAT3, HEAT4, FANS};

// Shorthand for setting a pin to output mode
#define GPOUT			BCM2835_GPIO_FSEL_OUTP

// Heat Pump Upper Limit
static const double PUMPHT = 90.0;
// Heat Pump Upper Hysteresis Trigger
static const double PUMPHSTR = 82.5;
// Lagering temps are between 1.666 C & 7.222 C (40 F = 4.444 C)
double desired_product_temp = 24.222;

///////////////////////////////////////////////////////////////////////////////
//			    FUNCTION PROTOTYPES				     //
///////////////////////////////////////////////////////////////////////////////

// Local initialization subroutine
int init_local();
// Web initialization subroutine
int init_web();
// Print command line help
void display_help();
// The temperature comparison and adjustment subroutine
void comp_temps();
// Function to turn on a device and log the operation
void turnON(int pin);
// Function to turn on all devices and log the operation
void turnOFF(int pin);
// Function to turn off a device and log the operation
void allON();
// Function to turn off all devices and log the operation
void allOFF();

///////////////////////////////////////////////////////////////////////////////
//			     GLOBAL VARIABLES				     //
///////////////////////////////////////////////////////////////////////////////

// This temperature array is defined in tempget.h
// Sensor location 0 should be the product temperature
double temps[NUM_SENS];
// This thermomter name string array is also defined in tempget.h
char thmtr[NUM_SENS][16];

///////////////////////////////////////////////////////////////////////////////
//				MAIN PROGRAM				     //
///////////////////////////////////////////////////////////////////////////////

int main (int argc, char *argv[]) {
	// Bind the clean-up function to program exit
	atexit(dbmsg_disconnect);


///////////////////////////////////////////////////////////////////////////////
	if (argc < 2) {
		// Initialize the controller
		init_local();
		// Determine heater posture based on current readings
		comp_temps();
		
///////////////////////////////////////////////////////////////////////////////
	} else {
		// Initialize the web controller
		init_web();
		// One time variables
		devStat state;
		int pin = 5;
		// Parse the input parameters
		while ((argc > 1) && (argv[1][0] == '-')) {
			switch (argv[1][1]) {
			case 'h':		// Heater
				pin = (int)argv[1][2] - '1';
				break;
			case 'f':		// Fan
				pin = 4;
				break;
			case 'r':		// Run
				state = ON;
				break;
			case 's':		// Stop
				state = OFF;
				break;
			case 'a':		// All on
				allON();
				return 0;
			case 'x':		// All stop
				allOFF();
				return 0;
			case 't':
				desired_product_temp = get_desired_temp();
				return 0;
			default:
				printf("Invalid parameters\n");
				display_help();
				return 0;
			}
			++argv;
			--argc;
		}
		if (state == OFF) {
			turnOFF(heats[pin]);
		} else {
			turnON(heats[pin]);
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void display_help() {
	printf("Usage: <program> (-[h,f] -[r,s]) or (-[a,x])\n");
	printf("\t-h\tThis will command a heater to change state,");
	printf("default is ON.\n");
	printf("\t-f\tThis will command a fan to change state,");
	printf("default is ON.\n");
	printf("\n");
	printf("\t-r\tThis will command a device to turn ON.\n");
	printf("\t-s\tThis will command a device to turn OFF.\n");
	printf("\n");
	printf("\t-a\tThis will command ALL devices to turn ON.\n");
	printf("\t-x\tThis will command ALL devices to turn OFF.\n");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int init_local() {
 	// Connect to SQL database
	dbmsg_connect();

	// Populate the temperature array
	get_temps();

	// Get Product Temperature Setting
	desired_product_temp = get_desired_temp();

	// Initialize the hardware interface
	if (!bcm2835_init()) {
		return -1;
	}

	// Set all appropriate GPIO pins to outputs
	bcm2835_gpio_fsel(HEAT1, GPOUT);
	bcm2835_gpio_fsel(HEAT2, GPOUT);
	bcm2835_gpio_fsel(HEAT3, GPOUT);
	bcm2835_gpio_fsel(HEAT4, GPOUT);
	bcm2835_gpio_fsel(FAN, GPOUT);

	// Determine message source
	source = LOCAL;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int init_web() {
 	// Connect to SQL database
	dbmsg_connect();

	// Initialize the hardware interface
	if (!bcm2835_init()) {
		return -1;
	}

	// Set all appropriate GPIO pins to outputs
	bcm2835_gpio_fsel(HEAT1, GPOUT);
	bcm2835_gpio_fsel(HEAT2, GPOUT);
	bcm2835_gpio_fsel(HEAT3, GPOUT);
	bcm2835_gpio_fsel(HEAT4, GPOUT);
	bcm2835_gpio_fsel(FAN, GPOUT);

	// Determine message source
	source = WEB;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void comp_temps() {
	// If the product temperature is below the threshold, stop pumping heat
	if (temps[0] <= desired_product_temp) {
		printf("COLD!\n");
		// Set the outputs to the heatpump relays to open
		allOFF();
		// Keep the fans running
		turnON(FANS);
	} else {
		printf("WARM!\n");
		// Cycle through the heatpumps
		int i;
		for (i = 1; i < 5; ++i)
		{
			// If the pump is too hot, turn it off, start cooldown
			if (temps[i] >= PUMPHT) {
				turnOFF(heats[i-1]);
			} else {
				// Otherwise turn the pump on
				printf("%d:%f\n",i,temps[i]);
				turnON(heats[i-1]);
			}
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void turnON(int pin) {
	// Check pin state for OFF
	if (bcm2835_gpio_lev(pin)) {
		// Turn a pin ON
		bcm2835_gpio_write(pin, LOW);
		// Determine Device ID and Type
		int i = 0;
		while(heats[i++] != pin) {}
		if (i == 5) {
			// Send a message to SQL database
			mod_device_stat(FAN, 1, ON);
		} else {
			// Send a message to SQL database
			mod_device_stat(HEATER, i, ON);
		}
	}


	return;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void turnOFF(int pin) {
	// Check pin state for ON
	if (!bcm2835_gpio_lev(pin)) {
		// Turn a pin OFF
		bcm2835_gpio_write(pin, HIGH);
		// Determine Device ID and Type
		int i = 0;
		while(heats[i++] != pin) {}
		if (i == 5) {
			// Send a message to SQL database
			mod_device_stat(FAN, 1, OFF);
		} else {
			// Send a message to SQL database
			mod_device_stat(HEATER, i, OFF);
		}
	}

	return;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void allON() {
	// Turn each pin ON
	int i;
	for (i = 0; i < 5; i++) {
		turnON(heats[i]);
	}

	return;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void allOFF() {
	// Turn each pin OFF
	int i;
	for (i = 0; i < 5; i++) {
		turnOFF(heats[i]);
	}

	return;
}
