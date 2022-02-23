#ifndef _TOKENIZE_H
#define _TOKENIZE_H

#include "vect.h"

/* Returns a vector of the tokens in the given input string. */
vect_t* tokenize(char* input);

#define MAX_STRLEN 300 /* Must be at least 255 */

#endif /* ifndef _TOKENIZE_H */
