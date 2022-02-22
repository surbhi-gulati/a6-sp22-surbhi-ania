#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenize.h"

// Adds the current word to the buffer iff it is not empty. 
static void add_cur_word(char* cur_word, vect_t* buf) {
	if (strlen(cur_word) > 0) {
		const char *token = cur_word;
		vect_add(buf, token);
	}
 }

// Add a quoted token from the given input to the given buffer vector, 
// starting at the given index. Do not include the closing quote. 
// Return the index following the closing quote.
static int quoted_token(char* input, vect_t* buf, char start_symbol, int fst_idx) {
        char* quote = "";
        int i = fst_idx + 1;
        while (strncmp(&input[i], "" + start_symbol, 1) != 0) {
                quote = strncat(quote, &input[i], sizeof(input[i]));
        }
	add_cur_word(quote, buf);

        return i + 1;
}

// Checks if an input string is a member of the passed string array.
// Returns 1 if true, 0 if false.
static int contains(char* array, char element) {
        for (int i = 0; i < (sizeof(array) / sizeof(array[0])); i++) {
                if (array[i] == element) {
                        return 1;
                }
        }
        return 0;
}

// Returns a vector of the tokens in the given input, capping tokenization at the maximum number
// of characters to read that is provided..
vect_t* tokenize(char* input, int max_tokens) {

	vect_t *buf = vect_new(); // stores all tokens
	char* cur_word = ""; // stores current word being built

	char special_symbols[6] = {'(', ')', '<', '>', ';', '|'}; // get own tokens if not quoted
	char white_spaces[2] = {' ', '\t'}; // whitespaces

	// iterate through string and collect and add tokens to buffer
	int i = 0; // current position in string
	while (input[i] != '\0' && i < max_tokens) { // while the end of string is not reached

		// first check if the current char is a quote
	  	if (strncmp(&input[i], "\"", 1) == 0 || strncmp(&input[i], "\'", 1) == 0) {
			// adds the current word stored into the buffer before processing next token
			add_cur_word(cur_word, buf);
			cur_word = "";

			// read following values till closing quote is found; progress counter i
			// to index following the closing quote
			i = quoted_token(input, buf, input[i], i);
		}

		// if current char is a special symbol
		// save current value as token & save current char as token in buffer
		else if (contains(special_symbols, input[i]) == 1) {
			add_cur_word(cur_word, buf);
			cur_word = "";

			// add special symbol as a token
			add_cur_word(&input[i], buf);
			i++;
		}
		
		// if encounter a whitespace character
		// save current value as token and proceed to next char
		// if no value is being built, simply proceed to next char
		else if (contains(white_spaces, input[i]) == 1) {
                        add_cur_word(cur_word, buf);
			cur_word = "";
			i++;
		}	

		// if encountering no special symbol or whitespace, value belongs to new word
		// create vector to build a word as part of a new token
		else {
			cur_word = strncat(cur_word, &input[i], sizeof(input[i]));
			i++;	
		}
	}

	// empty the current word in to buffer
	add_cur_word(cur_word, buf);
	free(cur_word);
	return buf;
}

// Main driver for tokenizer: tokenize given string, then print all tokens up to 
// max number of tokens allowed.
int main(int argc, char **argv) {
	// transform **argv into array of chars
	char expr[MAX_STRLEN];
	strncpy(expr, argv[1], strlen(argv[1])); // copy given string argument in to expr
	
	// call tokenize to create a vector of tokens
	vect_t *tokens = tokenize(expr, MAX_STRLEN);
		
	// print all of the tokens in vect_t
	for (int i = 0; i < vect_size(tokens); i++) {
		printf("%s\n", vect_get_copy(tokens, i));
	}
	free(tokens);

	return 0;
}
