/**
 * CS3600, Spring 2014
 * Project 3 Helper Functions
 * (c) 2014 Michelle Suk, Josh Caron
 */

#ifndef __3600HELPER_H__
#define __3600HELPER_H__
#include "debug.h"
#include "string.h"



/**
 * Determines what the given argument is and its validity.
 * @param  arg
 * @return
 *	Valid arguments
 * 		0		The argument is '-ns'
 * 		1		The argument is '-mx'
 * 		2 		The argument is a valid IP address
 *	Invalid arguments
 *		-1		Not an IP address or a flag
 *		-2 		Unrecognized flag
 *		-3 		No IP address given
 */
int determine_first_arg(char *arg);
int determine_first_arg(char *arg) {
	char c = arg[0];
	if (c == '@') {
		// It's an IP address - validate it
		int validated = validate_ip(arg);
		return validated;
	} else if (c == '-') {
		// It's a flag
		int validated = validate_flag(arg);
		return validated;
	} else {
		// Unrecognized first argument.
		return -1;
	}
}

int validate_flag(char *arg);
int validate_flag(char *arg) {
	if (strnlen(arg, 4) > 3) {
		// Flag is too long
		return -2;
	} else if (strcmp("-ns", arg) == 0) {
		return 0;
	} else if (strcmp("-mx", arg) == 0) {
		return 1;
	} else {
		return -2;
	}
}

int validate_ip(char *arg);
int validate_ip(char *arg) {
	char *delim = ":";
	char *addr = (char *) calloc(strlen(arg), sizeof(char));
	char *ip;
	char *port;
	ip = strtok(addr, delim);
	port = strtok(NULL, delim);

	if (ip == NULL) {
		// Error: No arguments were given - given an empty string
		return -3;
	} else {
		// Validate individual IP address parts
	}

	if (port != NULL) {
		// Check to make sure that the port is within valid port range	
	}

	return 2;
}

int validate_int_in_range(char *num, long min, long max);
int validate_int_in_range(char *num, long min, long max) {
	int base = 10; // decimal notation
	char *endptr;
	
	// Add in the if-statement for ERRNO to check that range fits for strtol

	long val = strtol(num, &endptr, base);

	if (endptr == num) {
		// Error: No numbers were found
	}
	if (val < min || val > max) {
		// Error: Out of range, not valid
	} 
}