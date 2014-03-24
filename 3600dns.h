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

bool create_packet(header *h, question *q, int name_size, char *packet);
bool create_packet(header *h, question *q, int name_size, char *packet) {
	char *pos = packet;
	
	memcpy(pos, h, sizeof(*h));
	pos += sizeof(*h);

	memcpy(pos, q->qname, name_size);
	pos += name_size;

	memcpy(pos, &q->qtype, sizeof(q->qtype));
	pos += sizeof(q->qtype);

	memcpy(pos, &q->qclass, sizeof(q->qclass));

	return true;
}

#endif

