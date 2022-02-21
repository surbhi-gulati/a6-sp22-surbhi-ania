#include <stdio.h>

#include "tokenize.h"

// main driver for tokenizer function
int main(int argc, char **argv) {
	
	// transform **argv into array of chars
	char expr[MAX_STRLEN];
	strncpy(expr, argv[0], strlen(argv[0])); // copy given string argument argv[0] in to expr

	// call tokenize to create a vector of tokens
	vect_t *tokens = tokenize(expr, MAX_STRLEN);

	// print all of the tokens in vect_t
	for (int i = 0; i < vect_size(tokens); i++) {
		printf(tokens[i]);
		printf("\n");
	}	

	return 0;
}
