/**
 * CS 3600, Spring 2014
 * Project 3 Code
 * (c) 2014 Joshua Caron, Michelle Suk
 */

#ifndef __3600VALIDATION_H__
#define __3600VALIDATION_H__
#include "datatypes.h"
#include "validation.c"

/**
 * Checks to see if the given IP is valid. There should be four parts,
 * each separated by a '.', and each number is valid from 0 to 255, inclusive.
 * @param  ip The IP address to validate
 * @return    True if it is valid, false otherwise
 */
bool validate_ip_addr(char *ip);

/**
 * Checks to see if the given port is in range. Valid port range is from
 * 0 to 65535, inclusive.
 * @param  port The port, as a string, to check
 * @return      True if it is, false otherwise.
 */
bool validate_port(char *port);

/**
 * Validate a full IP address, including a port number (optional)
 * @param  ip The IP address to check
 * @return    True if all parts are valid, false otherwise
 */
bool validate_ip(char *ip);

#endif
