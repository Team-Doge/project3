/*
 * CS3600, Spring 2014
 * Project 2 Starter Code
 * (c) 2013 Alan Mislove
 *
 */

#ifndef __3600DNS_H__
#define __3600DNS_H__
#include "debug.h"
#include "datatypes.h"
#include "extraction.h"
#include "validation.h"

void create_packet(header *h, question *q, int name_size, unsigned char *packet);
int print_ans(header *h, answer *ans, unsigned char *response);

void create_packet(header *h, question *q, int name_size, unsigned char *packet) {
	unsigned char *pos = packet;
	
	memcpy(pos, h, sizeof(*h));
	pos += sizeof(*h);

	memcpy(pos, q->qname, name_size);
	pos += name_size;

	memcpy(pos, &q->qtype, sizeof(q->qtype));
	pos += sizeof(q->qtype);

	memcpy(pos, &q->qclass, sizeof(q->qclass));
}

int print_ans(header *h, answer *ans, unsigned char *response) {
	char *auth = h->aa ? "auth" : "nonauth";
	if (ans->type == 1) {
		// A Record
		char *buf = (char *) calloc(16, sizeof(char));
		extract_ip(ans->rdata, buf, ans->rdlength);
		printf("IP\t%s\t%s\n", buf, auth);
	} else if (ans->type == 5) {
		// CNAME
		char *buf = (char *) calloc(1000, sizeof(char));
		extract_alias(ans->rdata, buf, ans->rdlength, response);
		printf("CNAME\t%s\t%s\n", buf + 1, auth);
	} else if (ans->type == 2) {
		// Name Server
		char *buf = (char *) calloc(1000, sizeof(char));
		extract_alias(ans->rdata, buf, ans->rdlength, response);
		printf("NS\t%s\t%s\n", buf + 1, auth);
		return -1;
	} else if (ans->type == 8) {
		// Mail Server
		return -1;
	} else {
		// Unknown type
		error("ERROR\tUnknown answer type.\n");
		return -1;
	}
	
	return 0;
}


#endif
