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
	printf("%d",request);
	switch (request) {
		case GET:
			q = "GET ";
			break;
		case HEAD:
			q = "HEAD ";
			break;
	}
	msg = strcat(msg, q);
	msg = strcat(msg, input);
	msg = strcat(msg, " HTTP/1.1\r\n");
	msg = strcat(msg, "Host: ");
	msg = strcat(msg, domain); 
	msg = strcat(msg, CRLF);
	msg = strcat(msg, CRLF);
	return msg;
}

/* This functions examines a HTTP header and checks for 
   chunked input, and returns 0 if the Transfer Encoding
   is chunked, or returns a positive integer describing 
   the remaining size if the header has a specified length
*/
int parsehead(char* resp, int size) {
	char* startnum, *end;
	int fullsize;
	int i;

	// First, copy the response to a temporary array since the 
	// response is not necessarilly null terminated
	char* temp = malloc(sizeof(char) * (size+1));
	for (i = 0; i < size; i++){
		temp[i] = resp[i];
	}
	temp[size] = 0; // Null terminate it
	printf("---------------------------------------\n%s\n------------------------------------------\n",temp);
	
	startnum = strstr(temp, "Content-Length");
	if (startnum != NULL) {
		printf("Content length is provided\n");
		while (!isdigit(*startnum)){
			startnum++;
		}
		end = startnum;
		while (*(end++) != '\n');
		fullsize = (int) strtol(startnum, &end, 10);
		end = findheadend(temp);
		free(temp);
		return (fullsize + sizeof(char) * (resp - end));
	}
	else {
		printf("Length is not provided\n");
		free(temp);
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
int processchunked(char* resp, int size) {
	char* p = resp + size-5; // sets p to the address of the last non-CRLF character
	printf("%d %d %d\n", *(p-1), *p, *(p+1));
	if (*p == '0'  && *(p-1) == '\n' && *(p+1) == '\r') {
		return 0;
	}
	return 1;	
}
