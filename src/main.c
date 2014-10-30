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

	int bytectr = 0;
	int descriptor;
	int bytes_sent;
	int bytes_received;
	int totalbytes;
	int i;

	struct addrinfo hints;
	struct addrinfo *res, *p;
	struct uri *input;

	char receive[MAXBUFFER];
	char* page = NULL; // Will contain the HTTP responses

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

	
	bytes_received = recv(descriptor, receive, MAXBUFFER, 0);
	printf("%d Bytes received\n", bytes_received);

	
	// If the page provides the length, follow this path
	if ((totalbytes = parsehead(receive, bytes_received)) != 0) {
		page = malloc(sizeof(char) * totalbytes); // Set the page to the size of the full page
		memset(page, 0, totalbytes); 
		while (bytes_received + bytectr < totalbytes) {
			for (i = 0; i < bytes_received; i++) {
				page[i + bytectr] = receive[i];
			}
		bytectr += bytes_received;
		bytes_received = recv(descriptor, receive, MAXBUFFER, 0);
		}
	}
	
	// If the content has a chunked length, then the code follows this path
	else {
		totalbytes = bytes_received;
		while (processchunked(receive, bytes_received)){  // Check for the errors or the end of the message
			page = realloc(page, sizeof(char) * (totalbytes + 1));
			for (i = 0; i < bytes_received + 1; i++) {
				page[i + bytectr] = receive[i];
			}
			bytectr += bytes_received;
			bytes_received = recv(descriptor, receive, MAXBUFFER, 0);
			printf("%d bytes received\n", bytes_received);
			totalbytes += bytes_received;
			printf("%d total bytes\n", totalbytes);
		}
		page = realloc(page, sizeof(char) * (totalbytes + 1));
		for (i = 0; i < bytes_received + 1; i++) {
			page[i + bytectr] = receive[i];
		}
	}
	page[totalbytes] = 0;
	
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
