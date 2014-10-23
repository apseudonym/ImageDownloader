#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define NUMCHARS 30 // The number of chars that will be added by a request
#define CHUNKED 1

char* getrequest(const char* input, const char* domain) {
	int size = sizeof(char) * (strlen(input) + strlen(domain));
	char* msg = calloc(size + NUMCHARS, sizeof(char));
	msg = strcat(msg, "GET ");
	msg = strcat(msg, input);
	msg = strcat(msg, " HTTP/1.1\r\n");
	msg = strcat(msg, "Host: ");
	msg = strcat(msg, domain); 
	msg = strcat(msg, "\r\n\r\n\0");
	return msg;
}

int parseresponse(char* resp, int respsize) {
	char* temp = malloc(sizeof(char) * (respsize + 1));
	*(temp + respsize) = 0; // make temp into a null terminated response
	if (strstr(temp, "Transfer-Encoding:")

	
}

int processresponse(char* dest, char* resp, int destsize, int respsize) {
	int i;
	dest = realloc(dest, sizeof(char) * (destsize + respsize + 1));
	if (destsize == 0) { // Check for first response to parse through the header

