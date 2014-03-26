#ifndef __3600DEBUG_H__
#define __3600DEBUG_H__
#include <stdbool.h>
#include <stdio.h>
#include "datatypes.h"

// global variable for triggering debugging
const int DEBUG = true;
// global variable for printing out errors
const int ERROR = true;

// debugging macro for printing to standard out
#define debug(...)\
if (DEBUG) {\
	fprintf(stdout, __VA_ARGS__); \
}

// debugging macro for printing to standard error
#define error(...) \
if (ERROR) { \
	fprintf(stderr, __VA_ARGS__); \
}

/**
 * Debug a header by printing out all its fields
 * @param h
 */
void debug_header(header *h);
void debug_header(header *h) {
	debug("\n\nDumping header information.\n");
	debug("\tID:\t\t%d\n", h->id);
	debug("\tQR:\t\t%d\n", h->qr);
	debug("\tOPCODE:\t\t%d\n", h->opcode);
	debug("\tAA:\t\t%d\n", h->aa);
	debug("\tTC:\t\t%d\n", h->tc);
	debug("\tRD:\t\t%d\n", h->rd);
	debug("\tRA:\t\t%d\n", h->ra);
	debug("\tZ:\t\t%d\n", h->z);
	debug("\tRCODE:\t\t%d\n", h->rcode);
	debug("\tQDCOUNT:\t%d\n", h->qdcount);
	debug("\tANCOUNT:\t%d\n", h->ancount);
	debug("\tNSCOUNT:\t%d\n", h->nscount);
	debug("\tARCOUNT:\t%d\n", h->arcount);
}

/**
 * Debug a question by printing out all of its fields
 * @param q
 */
void debug_question(question *q);
void debug_question(question *q) {
	debug("\n\nDumping question information.\n");
	debug("\tQNAME:\t\t%s\n", q->qname);
	debug("\tQTYPE:\t\t%d\n", q->qtype);
	debug("\tQCLASS:\t\t%d\n", q->qclass);
}

/**
 * Debug an answer by printing out all of its fields, except for RDATA
 * @param ans [description]
 */
void debug_answer(answer *ans);
void debug_answer(answer *ans) {
	debug("\n\nDumping answer information.\n");
	debug("\tNAME:\t\t%s\n", ans->name);
	debug("\tTYPE:\t\t%d\n", ans->type);
	debug("\tCLASS:\t\t%d\n", ans->class);
	debug("\tTTL:\t\t%d\n", ans->ttl);
	debug("\tRDLENGTH:\t%d\n", ans->rdlength);
}

#endif
