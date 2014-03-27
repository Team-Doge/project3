/**
 * CS 3600, Spring 2014
 * Project 3 Code
 * (c) 2014 Joshua Caron, Michelle Suk
 */

#include "3600dns.h"


bool validate_port(char *port) {
	int val = port ? atoi(port) : 0;
	bool valid = (0 <= val) && (val <= 65535);
	if (!valid) {
		error("ERROR\tPort out of range.\n");
	}
	return valid;
}

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


bool validate_ip(char *ip) {
    char *ip_copy = (char *) calloc(strlen(ip), sizeof(char));
    strcpy(ip_copy, ip);
    char *ip_addr = strtok(ip_copy, ":");
    char *port = strtok(NULL, ":");
    bool valid = validate_ip_addr(ip_addr) && validate_port(port);
    free(ip_copy);
    return valid;
}
