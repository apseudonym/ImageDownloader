#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* getRequest(const char* input, const char* domain) {
	char* msg = (char*) malloc(sizeof input + (sizeof(char)) * 20); // Add some extra space for the request
	memset(msg, 0, (sizeof msg));
	msg = strcat(msg, "GET ");
	msg = strcat(msg, input);
	msg = strcat(msg, " HTTP/1.1\n");
	msg = strcat(msg, "Host: ");
	msg = strcat(msg, domain); 
	msg = strcat(msg, "\n");
	return msg;
}
