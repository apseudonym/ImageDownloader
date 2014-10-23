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

	char* message = getrequest(input->path, input->domain); // Generates the HTTP request
	freeURI(input); // At this point, the user's initial input is no longer needed

	if ((bytes_sent = send(descriptor, message,
			(sizeof(char) * strlen(message)), 0)) != -1) {
		printf("%s\n", message);
		printf("%d bytes sent out of %d\n", bytes_sent,
			   	(int)(sizeof(char) * strlen(message)));
	}
	free(message);

	char* page = NULL; // Will contain the whole page
	int totalbytes = 0;
	int size;
	
	bytes_received = recv(descriptor, receive, MAXBUFFER, 0);
	totalbytes += bytes_received;
	
	while ((size = processresponse(page, receive, totalbytes, bytes_received) != 0)){  // Check for the errors or the end of the message
		totalbytes += bytes_received;
		bytes_received = recv(descriptor, receive, size, 0);
	}

	
	*(page + bytectr) = 0; // Adds a null terminating character
	if (bytes_received == -1) {
		printf("Error\n");
	}
	else {
		printf("Bytes received: %d\n", totalbytes);
		printf("%s\n",page);
		free(page);
	}

	return 0;
}
