/**
 * CS 3600, Spring 2014
 * Project 3 Code
 * (c) 2014 Joshua Caron, Michelle Suk
 */

#ifndef __3600EXTRACTION_H__
#define __3600EXTRACTION_H__
#include "datatypes.h"
#include "extraction.c"
/**
 * Extract a name from the given response and write it to the given buffer
 * @param  response   The full response containing data
 * @param  ans_offset The offset to begin reading data from
 * @param  buf        The buffer to write data into
 * @return            The number of bytes written into the buffer
 */
int extract_name(unsigned char *response, int ans_offset, unsigned char *buf);


/**
 * Extract the information from the given response and put it into the given
 * header
 * @param response The response containing data, starting where the header should begin
 * @param h        A pointer to a header struct to write to
 */
void extract_header(unsigned char *response, header *h);

/**
 * Extract the information from the given response and put it into the given
 * question
 * @param response The response containing data, starting where the question should begin
 * @param q        A pointer to a question struct to write to
 * @param query    The query that was asked
 */
void extract_question(unsigned char *response, question *q, char *query);


/**
 * Extract the information from the given response and put it into the given 
 * answer
 * @param  response   The full response containing data
 * @param  ans        A pointer to an answer struct to write to
 * @param  ans_offset The offset to begin extracting data from
 * @return            The number of bytes that were part of the answer's NAME field
 */
int extract_answer(unsigned char *response, answer *ans, int ans_offset);


/**
 * Extract an IP address from the given data and write it into the given buffer
 * @param data   The data that contains an IP address
 * @param buf    The buffer to write in to
 * @param length The maximum number of bytes to read from the data
 */
void extract_ip(unsigned char *data, char *buf, unsigned short length);

/**
 * Extract a CNAME alias from the given data and write it to the given buffer
 * @param data     The data containing the beginning of the alias
 * @param buf      The buffer to write data to
 * @param length   The maximum length of data to read
 * @param response A pointer to the full response
 */
void extract_alias(unsigned char *data, char *buf, unsigned short length, unsigned char *response);

void extract_mail_server(unsigned char *data, char *buf, unsigned int *preference, unsigned short length, unsigned char *response);

#endif
