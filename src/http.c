#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "http.h"

#define NUMCHARS 30 // The number of chars that will be added by a request

char* genrequest(const char* input, const char* domain, int request) {
	int size = sizeof(char) * (strlen(input) + strlen(domain));
	char* q;
	char* msg = calloc(size + NUMCHARS, sizeof(char));
	switch (request) {
		case GET:
			q = "GET ";
		case HEAD:
			q = "HEAD ";
	}
	msg = strcat(msg, q);
	msg = strcat(msg, input);
	msg = strcat(msg, " HTTP/1.1\r\n");
	msg = strcat(msg, "Host: ");
	msg = strcat(msg, domain); 
	msg = strcat(msg, CRLF);
	return msg;
}

/* This functions examines a HTTP header and checks for 
   chunked input, and returns 0 if the Transfer Encoding
   is chunked, or returns a positive integer describing 
   the remaining size if the header has a specified length
*/
int parsehead(char* resp) {
	char* startnum, *end;
	int fullsize;
	startnum = strstr(resp, "Content-Length");
	if (startnum != NULL) {
		printf("Content length is provided");
		while (!isdigit(*startnum)){
			startnum++;
		}
		end = startnum;
		while (*(end++) != '\n');
		fullsize = (int) strtol(startnum, &end, 10);
		end = findheadend(resp);
		return (fullsize + sizeof(char) * (strlen(resp) - strlen(end)));
	}
	else {
		printf("Length is not provided");
		return 0;
	}
}

char* findheadend(char* resp) {
	int i;
	for (i = 0; i < strlen(resp)-1; i++){
		if (resp[i] == '\n' && resp[i+1] == '\r') {
			i += 3;
			return resp + i;
		}
	}
	return NULL;
}

// Check if the server has finished responding. Only works for chunked inputs.
int processchunked(char* resp) {
	int len = strlen(resp);
	char* p = resp + len - 2; // sets p to the address of the last non-CRLF character
	if (*p == '0' && *(p-1) == '\n') {
		return 0;
	}
	return 1;	
}
