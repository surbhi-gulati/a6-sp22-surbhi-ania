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
static int quoted_token(char* input, vect_t* buf, int fst_idx) {
        char quote[MAX_STRLEN] = "\0";
        int i = fst_idx + 1;
        while (strncmp(&input[i], "\"", 1) != 0) {
		// error out if the quote does not end
		if (strncmp(&input[i], "\0", 1) == 0) {
			exit(1);
		}
                strncat(quote, &input[i], sizeof(input[i]));
		i++;
        }
	add_cur_word(quote, buf);

        return i + 1;
}

// Checks if given char is a special symbol (1 = true, 0 = false);
static int is_special(char element) {
	const char special_symbols[] = {'(', ')', '<', '>', ';', '|'};  // get own tokens if not quoted
        for (int i = 0; i < (sizeof(special_symbols) / sizeof(*special_symbols)); i++) {
                if (strncmp(&special_symbols[i], &element, 1) == 0) {
                        return 1;
                }
        }
        return 0;
}

// Returns a vector of the tokens in the given input string.
vect_t* tokenize(char* input) {

	vect_t *buf = vect_new(); // stores all tokens
	char cur_word[MAX_STRLEN] = "\0"; // stores current word being built

	// iterate through string and collect and add tokens to buffer
	int i = 0; // current position in string
	while (i < MAX_STRLEN && input[i] != '\0') { // while the end of string is not reached

		// first check if the current char is a quote
	  	if (strncmp(&input[i], "\"", 1) == 0) {
			// adds the current word stored into the buffer before processing next token
			add_cur_word(cur_word, buf);
			memset(cur_word, '\0', MAX_STRLEN);

			// read following values till closing quote is found; progress counter i
			// to index following the closing quote
			i = quoted_token(input, buf, i);
		}

		// if current char is a special symbol
		// save current value as token & save current char as token in buffer
		else if (is_special(input[i]) == 1) {
			// adds the current word stored into the buffer before processing next token
			add_cur_word(cur_word, buf);
			memset(cur_word, '\0', MAX_STRLEN);

			// add special symbol as a token
			strncat(cur_word, &input[i], sizeof(input[i]));
			add_cur_word(cur_word, buf);
			memset(cur_word, '\0', MAX_STRLEN);
			i++;
		}
		
		// if encounter a whitespace character
		// save current value as token and proceed to next char
		// if no value is being built, simply proceed to next char
		else if (strncmp(&input[i], " ", 1) == 0) {
                        add_cur_word(cur_word, buf);
			memset(cur_word, '\0', MAX_STRLEN);
			i++;
		}	

		// if encounter a tab sequence '\t'
		// save current value as token and proceed to next char
		// if no value is being built, simply proceed to next char
		else if (strncmp(&input[i], "\\", 1) == 0
				&& (i + 1) < MAX_STRLEN
				&& input[i + 1] != '\0'
				&& strncmp(&input[i + 1], "t", 1) == 0) {
                        	add_cur_word(cur_word, buf);
                        	memset(cur_word, '\0', MAX_STRLEN);
                        	i = i + 2;
                }
		

		// if encountering no special symbol or whitespace, value belongs to new word
		// create vector to build a word as part of a new token
		else {
			strncat(cur_word, &input[i], sizeof(input[i]));
			i++;	
		}
	}

	// empty the current word in to buffer
	add_cur_word(cur_word, buf);
	return buf;
}

