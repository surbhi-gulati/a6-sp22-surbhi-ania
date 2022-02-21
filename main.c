#include <stdio.h>

#include "tokenize.h"

int main(int argc, char **argv) {

	vect_t *buf = vect_new(); // stores all tokens
	// TODO: create helper func to copy contents in to a string that will be added to buf
	vect_t *cur_word = vect_new(); // stores current word being built

	char expr[MAX_STRLEN];
	strncpy(expr, argv[0], strlen(argv[0])); // copy given string argument argv[0] in to expr

	char special_symbols[6] = {'(', ')', '<', '>', ';', '|'}; // get own tokens if not quoted
	char white_spaces[2] = {' ', '\t'}; // whitespaces

	// iterate through string and collect and add tokens to buffer
	int i = 0; // current position in string
	while (expr[i] != '\0' && i < MAX_STRLEN) { // while the end of string is not reached
		// first check if the current char is a quote
	  	if is_quote(expr[i]) {
			// read the following values until another quote found
			// // and add the string between first and next quote to the buffer
			// // set i = next index a quote is found + 1 to parse next token
			i = quoted_token(&expr[i], buf, i);
		}

		// if current char is a special symbol
		// save current value as token & save current char as token in buffer
		else if {
			//
		}
		
		// if encounter a whitespace character
		// save current value as token and proceed to next char
		// if no value is being built, simply proceed to next char
		else if {
			// 
		}	

		// if encountering no special symbol or whitespace, value belongs to new word
		// create vector to build a word as part of a new token
		else {
			//
		}
	}

	// empty the current word in to buffer (TODO: if it is present, only)
	vect_add(buf, *cur_word);

	free(buf);
	free(cur_word);
	return 0; 
}

// Add a quoted token from the given input to the given buffer vector, 
// starting at the given index. Do not include the closing quote. 
// Return the index following the closing quote.
int quoted_token(**input, buffer, fst_idx) {
	return 0;
}
