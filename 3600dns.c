/*
 * CS3600, Spring 2014
 * Project 3 Starter Code
 * (c) 2013 Alan Mislove
 *
 */

#include <math.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "3600dns.h"
#include "extraction.h"

/**
 * This function will print a hex dump of the provided packet to the screen
 * to help facilitate debugging.  In your milestone and final submission, you 
 * MUST call dump_packet() with your packet right before calling sendto().  
 * You're welcome to use it at other times to help debug, but please comment those
 * out in your submissions.
 *
 * DO NOT MODIFY THIS FUNCTION
 *
 * data - The pointer to your packet buffer
 * size - The length of your packet
 */
static void dump_packet(unsigned char *data, int size) {
	unsigned char *p = data;
	unsigned char c;
	int n;
	char bytestr[4] = {0};
	char addrstr[10] = {0};
	char hexstr[ 16*3 + 5] = {0};
	char charstr[16*1 + 5] = {0};
	for(n=1;n<=size;n++) {
			if (n%16 == 1) {
					/* store address for this line */
					snprintf(addrstr, sizeof(addrstr), "%.4x",
						 ((unsigned int)p-(unsigned int)data) );
			}
					
			c = *p;
			if (isprint(c) == 0) {
					c = '.';
			}

			/* store hex str (for left side) */
			snprintf(bytestr, sizeof(bytestr), "%02X ", *p);
			strncat(hexstr, bytestr, sizeof(hexstr)-strlen(hexstr)-1);

			/* store char str (for right side) */
			snprintf(bytestr, sizeof(bytestr), "%c", c);
			strncat(charstr, bytestr, sizeof(charstr)-strlen(charstr)-1);

			if(n%16 == 0) { 
					/* line completed */
					printf("[%4.4s]   %-50.50s  %s\n", addrstr, hexstr, charstr);
					hexstr[0] = 0;
					charstr[0] = 0;
			} else if(n%8 == 0) {
					/* half line: add whitespaces */
					strncat(hexstr, "  ", sizeof(hexstr)-strlen(hexstr)-1);
					strncat(charstr, " ", sizeof(charstr)-strlen(charstr)-1);
			}
			p++; /* next byte */
	}

	if (strlen(hexstr) > 0) {
			/* print rest of buffer if not empty */
			printf("[%4.4s]   %-50.50s  %s\n", addrstr, hexstr, charstr);
	}
}

int main(int argc, char *argv[]) {
	/**
	 * I've included some basic code for opening a socket in C, sending
	 * a UDP packet, and then receiving a response (or timeout).  You'll 
	 * need to fill in many of the details, but this should be enough to
	 * get you started.
	 */
	if (!(argc == 3 || argc == 4)) {
		error("ERROR\tIncorrect number of arguments given.\nExpected format:\t./3600dns [-ns|-mx] @<server:port> <name>\n");
		return -1;
	}
	// process the arguments
	bool is_mx = false;
	bool is_ns = false;
	char *ip_addr, *name;
	int port = 53;
	switch (argv[1][0]) {
		case '@':
			// IP address
			ip_addr = argv[1];
			name = argv[2];
			break;
		case '-':;
			// some flag
			char *flag = argv[1];
			if (strcmp(flag, "-mx") == 0) {
				is_mx = true;
			} else if (strcmp(flag, "-ns") == 0) {
				is_ns = true;
			} else {
				error("ERROR\tUnknown flag given.\nExpected format:\t./3600dns [-ns|-mx] @<server:port> <name>\n");
				return -1;
			}
			ip_addr = argv[2];
			name = argv[3];
			break;
		default:
			// error
			return -1;
	}

	char *ip_addr_copy = (char *) malloc(strlen(ip_addr));
	strcpy(ip_addr_copy, ip_addr);

	// Validate the IP Address
	bool valid = validate_ip(ip_addr+1);
	if (!valid) {
		return -1;
	}

	char *ip = strtok(ip_addr_copy, ":");
	char *port_str = strtok(NULL, ":");

	if (port_str != NULL) {
		port = atoi(port_str);
	}

	// construct the DNS request
	header head;
	head.id = htons(1337);
	head.qr = 0;
	head.opcode = 0;
	head.aa = 0;
	head.tc = 0;
	head.rd = 1;
	head.ra = 0;
	head.z = 0;
	head.rcode = 0;
	head.qdcount = htons(1);
	head.ancount = 0;
	head.nscount = 0;
	head.arcount = 0;
	//debug_header(&head);

	question q;
	q.qclass = htons(0x001);
	if (is_ns) {
		q.qtype = htons(0x002);
	} else (is_mx) {
		q.qtype = htons(0x00f);
	} else {
		q.qtype = htons(0x001);
	}

	char *name_copy = (char *) malloc(strlen(name));
	strcpy(name_copy, name);
	char *lookup = (char *) malloc(strlen(name) + 2);
	char *token = strtok(name_copy, ".");

	int position = 0;
	while (token != NULL) {
		int length = strlen(token);
		lookup[position] = (char) length;
		position++;

		for (int i = 0; i < length; i++) {
			lookup[position] = token[i];
			position++;
		}

		token = strtok(NULL, ".");
	}

	lookup[position] = (char) 0;
	q.qname = (unsigned char *) lookup;
	//debug_question(&q);
	
	int packet_size = sizeof(head) + strlen(lookup) + 1 + sizeof(q.qtype) + sizeof(q.qclass);
	unsigned char *packet = (unsigned char *) malloc(packet_size);
	create_packet(&head, &q, strlen(lookup) + 1, packet);
	dump_packet(packet, packet_size);

	// send the DNS request (and call dump_packet with your request)
	
	// first, open a UDP socket  
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// next, construct the destination address
	struct sockaddr_in out;
	out.sin_family = AF_INET;
	out.sin_port = htons(port);
	out.sin_addr.s_addr = inet_addr(ip + 1);

	if (sendto(sock, packet, packet_size, 0, &out, sizeof(out)) < 0) {
		// an error occurred
		error("Error has occurred sending the packet.\n");
	}

	// wait for the DNS reply (timeout: 5 seconds)
	struct sockaddr_in in;
	socklen_t in_len;

	// construct the socket set
	fd_set socks;
	FD_ZERO(&socks);
	FD_SET(sock, &socks);

	// construct the timeout
	struct timeval t;
	t.tv_sec = 5;
	t.tv_usec = 0;

	unsigned int input_size = 65535;
	unsigned char *input = (unsigned char *) calloc(input_size, sizeof(char));
	// wait to receive, or for a timeout
	if (select(sock + 1, &socks, NULL, NULL, &t)) {
		if (recvfrom(sock, input, input_size, 0, &in, &in_len) < 0) {
			// an error occured
			error("ERROR\tAn error occurred connecting.\n");
			return -1;
		}
	} else {
		// a timeout occurred
		printf("NORESPONSE\n");
		return 0;
	}

	// print out the result
	header ans_h;
	//dump_packet(input, 500);
	extract_header(input, &ans_h);
	//debug_header(&ans_h);
	switch (ans_h.rcode) {
		case 0:
			break;
		case 1:
			error("ERROR\tFormat Error: The name server was unable to interpret the query.\n");
			return 1;
		case 2:
			error("ERROR\tServer Failure: The name server was unable to process this query due to a problem with the name server.\n");
			return 2;
		case 3:
			printf("NOTFOUND\n");
			return 3;
		case 4:
			error("ERROR\tNot Implemented: The name server does not support the requested kind of query.\n");
			return 4;
		case 5:
			error("ERROR\tRefused: The name server refuses to perform the specified operation for policy reasons.\n");
			return 5;
		default:
			error("ERROR\tAn unknown error occurred.\n");
			return -1;
	}

	question ans_q;
	extract_question( input + sizeof(ans_h), &ans_q, lookup);
	//debug_question(&ans_q);

	// debug_answer(&ans);

	int input_pos = 0;
	input_pos += sizeof(ans_h);
	input_pos += sizeof(int) + strlen(lookup) + 1;
	for (int i = 0; i < ans_h.ancount; i++) {
		answer ans;
		int name_offset = extract_answer(input, &ans, input_pos);
		input_pos += name_offset + ans.rdlength + 10;
		print_ans(&ans_h, &ans, input);
	}
	return 0;	
}
