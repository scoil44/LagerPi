#ifndef DBMSG
#define DBMSG

/* Simple C program that connects to MySQL Database server */

#include <mysql.h>
#include <stdio.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////////////
//				   Command Log Table				      //
////////////////////////////////////////////////////////////////////////////////////////

// Action Results
typedef enum {FAIL, PASS} mod_dRslt;
const char rsltString[2][5] = {"FAIL", "PASS"};

// Command Sources
typedef enum {LOCAL,WEB,TEST} srcType;
const char srcName[3][15] = {"Local Admin", "Web Admin", "TEST ADMIN"};

////////////////////////////////////////////////////////////////////////////////////////
//				    Device Status Table				      //
////////////////////////////////////////////////////////////////////////////////////////

typedef enum {HEATER, FAN, SENSOR} devType;
const char devName[3][7] = {"Heater", "Fan", "Sensor"};

typedef enum {OFF, ON} devStat;
const char devStatString[2][4] = {"OFF", "ON"};

////////////////////////////////////////////////////////////////////////////////////////
//				SQL Database Declarations			      //
////////////////////////////////////////////////////////////////////////////////////////

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
char *server = "";
char *user = "";
char *password = "";
char *database = "";

////////////////////////////////////////////////////////////////////////////////////////
//				   Function Prototypes				      //
////////////////////////////////////////////////////////////////////////////////////////

// Connect to the database
extern void dbmsg_connect();
// Disconnect from the database
extern void dbmsg_disconnect();
// Adds a sensor data log entry
extern int add_sensor_log(int sens, float tmpr);
// Adds a command log entry
int add_command_log(mod_dRslt PassFail, devType device, int id, devStat state);
// Changes a device status
extern int mod_device_stat(devType devT, int id, devStat state);
// Gets the desired temp from the database
extern double get_desired_temp();

////////////////////////////////////////////////////////////////////////////////////////
//				    GLOBAL VARIABLES				      //
////////////////////////////////////////////////////////////////////////////////////////

// Command Source Variable
srcType source = TEST;

// Sensor Number
int msg_sensor = 9;
// Sensor Temperature
float msg_temp = 99.111;

////////////////////////////////////////////////////////////////////////////////////////
//				  UTILITY FUNCTIONS FOLLOW				      //
////////////////////////////////////////////////////////////////////////////////////////

void dbmsg_connect() {
	// Create a MYSQL connection
	conn = mysql_init(NULL);
	// Link that connection to the database
	if (!mysql_real_connect(conn, server,
		user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
	}
}

void dbmsg_disconnect() {
	// Free up any results remaining in memory
	mysql_free_result(res);
	// close connection
	mysql_close(conn);
}

// SQL Query to insert new sensor data based on global sensor and temp variables
int add_sensor_log(int sens, float tmpr) {
	// Build Query
	char query[100] = "";
	sprintf(query, "insert into `Sensor_Log` (`Sensor`,`Temp`) values (%1d,%3.2f)",sens,tmpr);

	// Print, Execute, and Display Result
	printf("SQL QUERY: %s\t", query);
	if (mysql_query(conn, query)) {
		fprintf(stderr, "SQL ERROR: %s\n", mysql_error(conn));
		return -1;
	} else {
		printf("SUCCESS!\n");
		return 0;
	}
}

// SQL Query to insert new entry into command log
int add_command_log(mod_dRslt PassFail, devType device, int id, devStat state) {
	// Build Action Log Message
	char actMsg[15];
	sprintf(actMsg, "%s,%d,%s", devName[device], id, devStatString[state]);

	// Build Query
	char query[200] = "";
	sprintf(query, "insert into `Command_Log` (`Action`,`Result`,`Source`) values ('%s','%s','%s')",actMsg,rsltString[PassFail],srcName[source]);

	// Print, Execute, and Display Result
	printf("SQL QUERY: %s\n", query);
	if (mysql_query(conn, query)) {
		fprintf(stderr, "SQL ERROR: %s\n", mysql_error(conn));
		return -1;
	} else {
		printf("SUCCESS!\n");
		return 0;
	}
}

// SQL Query to modify a device status
int mod_device_stat(devType devT, int id, devStat state) {
	// Build Query
	char query[200] = "";
	sprintf(query, "UPDATE `Device_Status` SET `State`=%d WHERE (`Device`='%s' AND `ID`=%d)",state,devName[devT],id);

	// Print, Execute, Display, and Return Result
	printf("SQL QUERY: %s\n", query);
	if (mysql_query(conn, query)) {
		fprintf(stderr, "SQL ERROR: %s\n", mysql_error(conn));
		add_command_log(FAIL, devT, id, state);
		return -1;
	} else {
		printf("SUCCESS!\n");
		add_command_log(PASS, devT, id, state);
		return 0;
	}
}

// SQL Query to get the desired product temperature
double get_desired_temp() {
	// Hold the temperature setting
	double temp_set;
	// Build Query
	char query[50] = "SELECT `Product Temp` FROM `Temperature_Vars`";
	
	printf("SQL QUERY: %s\n", query);
	if (mysql_query(conn, query)) {
		fprintf(stderr, "SQL ERROR: %s\n", mysql_error(conn));
		return -999;
	} else {
		printf("SUCCESS!\n");
		
		res = mysql_use_result(conn);
		row = mysql_fetch_row(res);
		temp_set = atof(row[0]);
		mysql_free_result(res);
		res = NULL;
		printf("Desired Temp = %f\n", temp_set);
		return temp_set;
	}
}

#endif