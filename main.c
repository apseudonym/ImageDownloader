#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "userin.h"
#include "http.h"

int main(int argc, char *argv[]){
	int descriptor;
	int bytes_sent;
	int bytes_received;
	struct addrinfo hints;
	struct addrinfo *res, *p;
	struct uri *test;

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

	if (argc < 1){
		printf("Err: Too few arguments");
		return 0;
	}

	test = getURI(argv[1]);
	if(getaddrinfo(test->domain, test->prefix, &hints, &res)){
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

	char* message = getRequest(test->path, test->domain);
	bytes_sent = send(descriptor, message, (sizeof(char) * strlen(message)), 0); 
	printf("%s\n", message);
	printf("%d bytes sent out of %d\n", bytes_sent, (int)(sizeof(char) * strlen(message)));

	void* receive = malloc((sizeof(int) * 100000000000));
	if ((bytes_received = recv(descriptor, receive, 3000, 0)) == -1) {
		printf("Error");
	}
	printf("%d bytes received.\nReceived: %s\n",bytes_received, (char*)receive);

	return 0;
}
