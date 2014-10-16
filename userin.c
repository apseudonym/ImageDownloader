#include<stdlib.h>
#include<stdio.h>

#include"userin.h"

struct uri* getURI(char* input) {

	char *temp, *start;
	struct uri *output = malloc(sizeof (struct uri));
//	printf("Full Input: %s\n",input);

	// Pulls the prefix
	start = temp = malloc(sizeof input);
	while(*input != ':'){
		*temp = *input;
		input++;
		temp++;
	}
	*temp = 0;
	output->prefix = start;

	input += 3; // Skips the 3 characters before the domain starts in the input

	// Pulls the main domain
	temp = malloc(sizeof input);
	start = temp;
	while(*input != '/'){
		*temp = *input;
		temp++;
		input++;
	}
	*temp = 0;
	output->domain = start;

	// Pulls the specific path
	temp = malloc(sizeof input);
	start = temp;
	while(*input){
		*temp = *input;
		temp++;
		input++;
	}
	*temp = 0;
	output->path = start;

	return output;
}
