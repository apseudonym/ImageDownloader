#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>

#include"userin.h"

int main(int argc, char *argv[]){
	int descriptor;
	struct addrinfo hints;
	struct addrinfo *res, *p;
	struct uri *test;
	char ipstr[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

	if (argc < 1){
		printf("Err: Too few arguments");
		return 0;
	}

	test = getURI(argv[1]);
	printf("Prefix: %s\n", test->prefix);
	printf("Domain: %s\n", test->domain);
	printf("Path: %s\n", test->path);
	if(getaddrinfo(test->domain, test->prefix, &hints, &res)){
		printf("Error with getaddrinfo");
	}
	
	for(p = res; p != NULL; p = p->ai_next){
	descriptor = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	}

	freeaddrinfo(res); // free the linked list
}
