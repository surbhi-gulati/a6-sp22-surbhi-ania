#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenize.h"

// Main driver for tokenizer: tokenize given string, then print all tokens up to 
// max number of tokens allowed.
int main(int argc, char *argv[]) {

	// transform **argv into array of chars
	char expr[MAX_STRLEN];
	fgets(expr, MAX_STRLEN, stdin); // copy given string argument in to expr
	expr[strcspn(expr, "\n")] = 0;
	
	// call tokenize to create a vector of tokens
	vect_t *tokens = tokenize(expr);
		
	// print all of the tokens in vect_t
	for (int i = 0; i < vect_size(tokens); i++) {
		char *token = vect_get_copy(tokens, i);
		printf("%s\n", token);
		free(token);
	}
	vect_delete(tokens);

	return 0;
}
