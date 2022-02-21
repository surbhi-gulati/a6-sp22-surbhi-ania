#include <stdio.h>

#include "tokenize.h"
// TODO: global variable MAX_STRLEN
// Returns a vect of the tokens in a String input
vect_t* tokenize(char* input) {

	vect_t *buf = vect_new(); // stores all tokens
	// TODO: create helper func to copy contents in to a string that will be added to buf
	char* cur_word = ""; // stores current word being built

	char special_symbols[6] = {'(', ')', '<', '>', ';', '|'}; // get own tokens if not quoted
	char white_spaces[2] = {' ', '\t'}; // whitespaces

	// iterate through string and collect and add tokens to buffer
	int i = 0; // current position in string
	while (input[i] != '\0' && i < MAX_STRLEN) { // while the end of string is not reached
		// first check if the current char is a quote
	  	if (input[i] == "\"" || input[i] == "\'") {
			// read the following values until another quote found
			// // and add the string between first and next quote to the buffer
			// // set i = next index a quote is found + 1 to parse next token
			i = quoted_token(&input, buf, i);
		}

		// if current char is a special symbol
		// save current value as token & save current char as token in buffer
		else if (contains(special_symbols, input[i]) == 1) {
			// need to reset cur_word
                        // add if full
			const char* old_word;
			strncpy(old_word, cur_word, strlen(cur_word) + 1);
			cur_word = "";
			vect_add(buf, old_word);

			const char* token;
			strncpy(token, input[i], strlen(input[i] + 1));
			vect_add(buf, token);
			i++;
			free(token);
			free(old_word);
		}
		
		// if encounter a whitespace character
		// save current value as token and proceed to next char
		// if no value is being built, simply proceed to next char
		else if (contains(white_spaces, input[i]) == 1) {
			// need to reset cur_word
			// add if full
			const char* token;
			strncpy(token, cur_word, strlen(cur_word) + 1);
                        cur_word = "";
			vect_add(buf, token);
			i++;
			free(token);
		}	

		// if encountering no special symbol or whitespace, value belongs to new word
		// create vector to build a word as part of a new token
		else {
			// need to update add(cur_word, input[i]) 
		}
	}

	// empty the current word in to buffer (TODO: if it is present, only)
	vect_add(buf, *cur_word);

	free(cur_word);
	return buf;
}

// Checks if an input string is a member of the passed string array
// Returns 1 if true, 0 if false
int contains(char* array, char element) {
	for (int i = 0; i < (sizeof array / sizeof array[0]); i++) {
		if (array[i] == element) {
			return 1;
		}
	}
	return 0;
}

// Add a quoted token from the given input to the given buffer vector, 
// starting at the given index. Do not include the closing quote. 
// Return the index following the closing quote.
int quoted_token(**input, buffer, fst_idx) {
	char* quote = "";
	int i = fst_idx + 1;
	while (input[i] != "\"" && input[i] != "\'") {
		quote = strcat(quote, input[i]);
	}
	const char* token = quote;
	vect_add(buf, token);

	return i + 1;
}
