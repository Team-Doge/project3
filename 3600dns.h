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

void extract_alias(unsigned char *data, char *buf, unsigned short length, char *response);
void extract_ip(unsigned char *data, char *buf, unsigned short length);

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

void extract_header(char *response, header *h);
void extract_header(char *response, header *h) {
	memcpy(h, response, sizeof(header));
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
int extract_answer(char *response, answer *ans, char *query, int ans_offset);
int extract_answer(char *response, answer *ans, char *query, int ans_offset) {
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
	return name_offset;
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

int print_ans(header *h, answer *ans, char *response);
int print_ans(header *h, answer *ans, char *response) {
	char *auth = h->aa ? "auth" : "nonauth";
	if (ans->type == 1) {
		// A Record
		char *buf = (char *) calloc(16, sizeof(char));
		extract_ip(ans->rdata, buf, ans->rdlength);
		printf("IP\t%s\t%s\n", buf, auth);
	} else if (ans->type == 5) {
		// CNAME
		char *buf = (char *) calloc(100, sizeof(char));
		extract_alias(ans->rdata, buf, ans->rdlength, response);
		printf("CNAME\t%s\t%s\n", buf + 1, auth);
	} else if (ans->type == 2) {
		// Name Server
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

void extract_ip(unsigned char *data, char *buf, unsigned short length) {
	int buf_pos = 0;
	for (int i = 0; i < length; i++) {
		sprintf(buf + buf_pos, "%u.", data[i]);
		buf_pos = strlen(buf);
	}
	buf[buf_pos-1] = '\0'; 
}

void extract_alias(unsigned char *data, char *buf, unsigned short length, char *response) {
	int buf_pos = 0;
	for (int i = 0; i < length; i++) {
		unsigned short p_len = data[i];

		if (p_len >= 192) {
			unsigned short offset = ((data[i] & 63) << 8) | data[i + 1];
			unsigned short length = strlen(&response[offset]);
			memcpy(&buf[buf_pos], &response[offset], length + 1);
			for (int j = 0; j < length; j++) {
				unsigned short p = buf[buf_pos + j];
				buf[buf_pos + j] = '.';
				j += p;
			}
			return;
		}
		buf[buf_pos] = '.';		
		buf_pos++;
		for (int j = 0; j < p_len; j++) {
			buf[buf_pos] = data[j + i + 1];
			buf_pos++;
		}
		i += p_len;
	}
	buf[buf_pos-1] = '\0';
}

#endif

