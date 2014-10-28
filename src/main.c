#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "userin.h"
#include "http.h"

#define MAXBUFFER 1000

int main(int argc, char *argv[]){

	int descriptor;
	int bytes_sent;
	int bytes_received;
	int totalbytes;

	struct addrinfo hints;
	struct addrinfo *res, *p;
	struct uri *input;

	char receive[MAXBUFFER];

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

	if (argc < 1){
		printf("Err: Too few arguments");
		return 0;
	}

	input = getURI(argv[1]);
	if(getaddrinfo(input->domain, input->prefix, &hints, &res)){
		printf("Error with getaddrinfo");
	}
	
	// loop through all the results and connect to the first we can
	for(p = res; p != NULL; p = p->ai_next) {
		if ((descriptor = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(descriptor, p->ai_addr, p->ai_addrlen) == -1) {
			close(descriptor);
			perror("client: connect");
			continue;
		}
		break;
	}

	freeaddrinfo(res); // free the linked list

	printf("Connected\n\n");

	char* message = genrequest(input->path, input->domain, GET); // Generates a GET request

	if ((bytes_sent = send(descriptor, message,
			(sizeof(char) * strlen(message)), 0)) != -1) {
		printf("%s\n", message);
		printf("%d bytes sent out of %d\n", bytes_sent,
			   	(int)(sizeof(char) * strlen(message)));
	}
	free(message);

	char* page = NULL; // Will contain the HTTP responses
	
	totalbytes = bytes_received = recv(descriptor, receive, MAXBUFFER, 0);
	printf("%d Bytes received", totalbytes);
	printf("%s", receive);
	
	// If the page provides the length, follow this path
	if (parsehead(receive)) {
		totalbytes = parsehead(receive);
		page = malloc(sizeof(char) * totalbytes); // Set the page to the size of the full page
		memset(page, 0, totalbytes); 
		do {
			strcat(page, receive);
			bytes_received += recv(descriptor, receive, MAXBUFFER, 0);
		}
		while (bytes_received < totalbytes);
	}
	
	// If the content has a chunked length, then the code follows this path
	else {
		int i;
		while (processchunked(receive)){  // Check for the errors or the end of the message
			page = realloc(page, sizeof(char) * totalbytes);
			for (i = 0; i < strlen(receive) + 1; i++) {
				page[i] = receive[i];
			}
			bytes_received = recv(descriptor, receive, MAXBUFFER, 0);
			totalbytes += bytes_received;
		}
	}

	
	if (bytes_received == -1) {
		printf("Error\n");
	}
	else {
		printf("Bytes received: %d\n", totalbytes);
		printf("%s\n",page);
		free(page);
	}

	freeURI(input); 
	return 0;
}
