#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define NUMCHARS 30 // The number of chars that will be added by a request

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
