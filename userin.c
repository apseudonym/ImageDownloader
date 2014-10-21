#include<stdlib.h>
#include<stdio.h>

#include"userin.h"

struct uri* getURI(char* input) {

	char *temp, *start, *p;
	struct uri *output = malloc(sizeof (struct uri));
//	printf("Full Input: %s\n",input);
	p = input;

	// Pulls the prefix
	start = temp = malloc(sizeof p);
	while(*p != ':'){
		*temp = *p;
		p++;
		temp++;
	}
	*temp = 0;
	output->prefix = start;

	p += 3; // Skips the 3 characters before the domain starts in the input

	// Pulls the main domain
	temp = malloc(sizeof p);
	start = temp;
	while(*p != '/'){
		*temp = *p;
		temp++;
		p++;
	}
	*temp = 0;
	output->domain = start;

	// Pulls the specific path
	temp = malloc(sizeof p);
	start = temp;
	while(*p){
		*temp = *p;
		temp++;
		p++;
	}
	*temp = 0;
	output->path = start;

	return output;
}

void freeURI(struct uri* uri){
	free(uri->prefix);
	free(uri->domain);
	free(uri->path);
}

