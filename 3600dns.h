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

void extract_header(char *response, ans_header *h);
void extract_header(char *response, ans_header *h) {
	memcpy(h, response, sizeof(ans_header));
	h->id = ntohs(h->id);
	h->qdcount = ntohs(h->qdcount);
	h->ancount = ntohs(h->ancount);
	h->nscount = ntohs(h->nscount);
	h->arcount = ntohs(h->arcount);
}

void extract_question(char *response, question *q, char *query);
void extract_question(char *response, question *q, char *query) {
	char *curr_pos = response;
	q->qname = curr_pos;
	curr_pos += strlen(query) + 1;

	memcpy(&q->qtype, curr_pos, sizeof(short));
	q->qtype = ntohs(q->qtype);
	curr_pos += sizeof(short);
	
	memcpy(&q->qclass, curr_pos, sizeof(short));
	q->qclass = ntohs(q->qclass);
}

void extract_answer(char *response, answer *ans, char *query);
void extract_answer(char *response, answer *ans, char *query) {
	char *curr_pos = response;
	ans->name = curr_pos;
	curr_pos += strlen(query) + 1;

	memcpy(&ans->type, curr_pos, sizeof(short));
	ans->type = ntohs(ans->type);
	curr_pos += sizeof(short);

	memcpy(&ans->class, curr_pos, sizeof(short));
	ans->class = ntohs(ans->class);
	curr_pos += sizeof(short);

	memcpy(&ans->ttl, curr_pos, sizeof(int));
	ans->ttl = ntohl(ans->ttl);
	curr_pos += sizeof(int);

	memcpy(&ans->rdlength, curr_pos, sizeof(short));
	ans->rdlength = ntohs(ans->rdlength);
	curr_pos += sizeof(short);

	ans->rdata = curr_pos;
}

#endif

