#ifndef __3600DEBUG_H__
#define __3600DEBUG_H__
#include "stdbool.h"

// global variable for triggering debugging
const int DEBUG = true;

// debugging macro for printing to standard out
#define debug(...)\
if (DEBUG) {\
	fprintf(stdout, __VA_ARGS__); \
}

// debugging macro for printing to standard error
#define error(...) \
if (DEBUG) { \
	fprintf(stderr, __VA_ARGS__); \
}
