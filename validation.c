/**
 * CS 3600, Spring 2014
 * Project 3 Code
 * (c) 2014 Joshua Caron, Michelle Suk
 */

#include "3600dns.h"

/**
 * Checks to see if the given port is in range. Valid port range is from
 * 0 to 65535, inclusive.
 * @param  port The port, as a string, to check
 * @return      True if it is, false otherwise.
 */
bool validate_port(char *port) {
	int val = port ? atoi(port) : 0;
	bool valid = (0 <= val) && (val <= 65535);
	if (!valid) {
		error("ERROR\tPort out of range.\n");
	}
	return valid;
}

/**
 * Checks to see if the given IP is valid. There should be four parts,
 * each separated by a '.', and each number is valid from 0 to 255, inclusive.
 * @param  ip The IP address to validate
 * @return    True if it is valid, false otherwise
 */
bool validate_ip_addr(char *ip) {
	char *ip_copy = (char *) calloc(strlen(ip), sizeof(char));
    strcpy(ip_copy, ip);
	int part_count = 0;
	int part_max = 4;
    char *delim = ".";
	char *token = strtok(ip_copy, delim);
	while (token != NULL) {
		if (part_count == part_max) {
			error("ERROR\tToo many parts to the IP Address.\n");
			return false;
		}
		int val = atoi(token);
		if (val < 0 || val > 255) {
			error("ERROR\tIP Address it out of range.\n");
			free(ip_copy);
			return false;
		}
		part_count++;
		token = strtok(NULL, delim);
	}
	if (part_count != part_max) {
		error("ERROR\tToo few parts to the IP Address.\n");
		return false;
	}

	return true;
}

/**
 * Validate a full IP address, including a port number (optional)
 * @param  ip The IP address to check
 * @return    True if all parts are valid, false otherwise
 */
bool validate_ip(char *ip) {
    char *ip_copy = (char *) calloc(strlen(ip), sizeof(char));
    strcpy(ip_copy, ip);
    char *ip_addr = strtok(ip_copy, ":");
    char *port = strtok(NULL, ":");
    bool valid = validate_ip_addr(ip_addr) && validate_port(port);
    free(ip_copy);
    return valid;
}
