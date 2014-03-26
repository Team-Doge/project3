#ifndef __3600DATATYPES_H__
#define __3600DATATYPES_H__

/**
 * A header for a packet to make a DNS query or answer.
 *
 * @param id        The id of the packet
 * @param rd        The bit for determing if recursion is desired (1) or not (0) when querying
 * @param tc        The bit for determing if a response was truncated (1) or not (0)
 * @param aa        The bit that indicates if an answer is authorative (1) or not (0)
 * @param opcode    The bits indicating the type of query
 * @param qr        The bit indicating whether the packet is a response (1) or not (0)
 * @param rcode     The response code for an answer
 * @param z         This field is unused, but exists for padding purposes
 * @param ra        The bit indicating if recursive query support is available (1) or not (0)
 * @param qdcount   The number of questions this packet contains
 * @param ancount   The number of answers this packet contains
 * @param nscount   The number of name server resource records in the authority records section
 * @param arcount   The number of resource records in the additonal records section
 */
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

/**
 * A question in a packet
 * 
 * @param qname     The name that is being queried
 * @param qclass    The number specifiying the the class of the query
 */
typedef struct question_t {
	unsigned char *qname;
	unsigned short qtype;
	unsigned short qclass;
} question;

/**
 * An answer in a packet
 *
 * @param type      The type of data in the RDATA field
 * @param class     The number specifiying the class of data in the RDATA field
 * @param ttl       The number of seconds the answer can be cached
 * @param rdlength  The length of the RDATA field
 * @param name      The domain name that was queried
 * @param rdata     The data the answer contains
 */
typedef struct answer_t {
	unsigned short type;
	unsigned short class;
	unsigned int ttl;
	unsigned short rdlength;
	unsigned char *rdata;
	unsigned char *name;
} answer;

#endif
