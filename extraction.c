/**
 * CS 3600, Spring 2014
 * Project 3 Code
 * (c) 2014 Joshua Caron, Michelle Suk
 */

#include "3600dns.h"


/**
 * Extract a name from the given response and write it to the given buffer
 * @param  response   The full response containing data
 * @param  ans_offset The offset to begin reading data from
 * @param  buf        The buffer to write data into
 * @return            The number of bytes written into the buffer
 */
int extract_name(unsigned char *response, int ans_offset, unsigned char *buf) {
	unsigned char *buf_pos = buf;
	int curr_buf_size = 0;

	unsigned char c = response[ans_offset];
	while (c != '\0') {
		unsigned short c_val = (unsigned short) c;
		if (c_val >= 192) {
			unsigned char c2 = response[ans_offset + 1];
			unsigned short offset = ((c_val & 63) << 8) | (unsigned short) c2;
			unsigned short length = strlen((char*) &response[offset]);
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

/**
 * Extract the information from the given response and put it into the given
 * header
 * @param response The response containing data, starting where the header should begin
 * @param h        A pointer to a header struct to write to
 */
void extract_header(unsigned char *response, header *h) {
	memcpy(h, response, sizeof(header));
	h->id = ntohs(h->id);
	h->qdcount = ntohs(h->qdcount);
	h->ancount = ntohs(h->ancount);
	h->nscount = ntohs(h->nscount);
	h->arcount = ntohs(h->arcount);
}

/**
 * Extract the information from the given response and put it into the given
 * question
 * @param response The response containing data, starting where the question should begin
 * @param q        A pointer to a question struct to write to
 * @param query    The query that was asked
 */
void extract_question(unsigned char *response, question *q, char *query) {
	unsigned char *curr_pos = response;
	q->qname = curr_pos;
	curr_pos += strlen(query) + 1;

	memcpy(&q->qtype, curr_pos, sizeof(short));
	q->qtype = ntohs(q->qtype);
	curr_pos += sizeof(short);
	
	memcpy(&q->qclass, curr_pos, sizeof(short));
	q->qclass = ntohs(q->qclass);
}

/**
 * Extract the information from the given response and put it into the given 
 * answer
 * @param  response   The full response containing data
 * @param  ans        A pointer to an answer struct to write to
 * @param  query      The query that was asked
 * @param  ans_offset The offset to begin extracting data from
 * @return            The number of bytes that were part of the answer's NAME field
 */
int extract_answer(unsigned char *response, answer *ans, int ans_offset) {
	unsigned char *curr_pos = response + ans_offset;
	unsigned int initial_buf_size = 100;
	unsigned char *buf = (unsigned char *) calloc(initial_buf_size, sizeof(char));
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


/**
 * Extract an IP address from the given data and write it into the given buffer
 * @param data   The data that contains an IP address
 * @param buf    The buffer to write in to
 * @param length The maximum number of bytes to read from the data
 */
void extract_ip(unsigned char *data, char *buf, unsigned short length) {
	int buf_pos = 0;
	for (int i = 0; i < length; i++) {
		sprintf(buf + buf_pos, "%u.", data[i]);
		buf_pos = strlen(buf);
	}
	buf[buf_pos-1] = '\0'; 
}

/**
 * Extract a CNAME alias from the given data and write it to the given buffer
 * @param data     The data containing the beginning of the alias
 * @param buf      The buffer to write data to
 * @param length   The maximum length of data to read
 * @param response A pointer to the full response
 */
void extract_alias(unsigned char *data, char *buf, unsigned short length, unsigned char *response) {
	int buf_pos = 0;
	for (int i = 0; i < length; i++) {
		unsigned short p_len = data[i];

		if (p_len >= 192) {
			unsigned short offset = ((data[i] & 63) << 8) | data[i + 1];
			unsigned short length = strlen((char *) &response[offset]);
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

void extract_mail_server(unsigned char *data, char *buf, unsigned short *preference, unsigned short length, unsigned char *response) {
	int buf_pos = 0;
	memcpy(preference, &data[buf_pos], sizeof(short));
    data += sizeof(short);
	for (int i = 0; i < length; i++) {
		unsigned short p_len = data[i];

		if (p_len >= 192) {
			unsigned short offset = ((data[i] & 63) << 8) | data[i + 1];
			unsigned short length = strlen((char *) &response[offset]);
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
