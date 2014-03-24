#ifndef __3600DATATYPES_H__
#define __3600DATATYPES_H__


typedef struct header_t {
	unsigned short id;
	unsigned short rd:1;
	unsigned short tc:1;
	unsigned short aa:1;
	unsigned short opcode:4;
	unsigned short qr:1;
	unsigned short rcode:4;
	unsigned short z:3;
	unsigned short ra:1;
	unsigned short qdcount;
	unsigned short ancount;
	unsigned short nscount;
	unsigned short arcount;
} header;

typedef struct question_t {
	char *qname;
	unsigned short qtype;
	unsigned short qclass;
} question;

typedef struct answer_t {
	char *name;
	unsigned short type;
	unsigned short class;
	unsigned int ttl;
	unsigned short rdlength;
	char *rdata;
} answer;

#endif