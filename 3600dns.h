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
int extract_name(char *response, int ans_offset, char *buf);
void extract_answer(char *response, answer *ans, char *query, int ans_offset);
void extract_answer(char *response, answer *ans, char *query, int ans_offset) {
	char *curr_pos = response + ans_offset;
	unsigned int initial_buf_size = 100;
	char *buf = (char *) calloc(initial_buf_size, sizeof(char));
	int name_offset = extract_name(response, ans_offset, buf);
	ans->name = buf;
	curr_pos += name_offset;

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

int extract_name(char *response, int ans_offset, char *buf) {
	char *buf_pos = buf;
	int curr_buf_size = 0;

	unsigned char c = response[ans_offset];
	while (c != '\0') {
		unsigned short c_val = (unsigned short) c;
		if (c_val >= 192) {
			unsigned char c2 = response[ans_offset + 1];
			unsigned short offset = ((c_val & 63) << 8) | (unsigned short) c2;
			unsigned short length = strlen(&response[offset]);
			memcpy(buf_pos, &response[offset], length + 1);
			curr_buf_size += 2;
			break;
		} else {
			memcpy(buf_pos, &response[ans_offset], c_val + 1);
			ans_offset += c_val + 1;
			buf_pos += c_val;
			curr_buf_size += c_val + 1;
		}
		c = response[ans_offset];
	}
	return curr_buf_size;
}

#endif

