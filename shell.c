#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"

/*
 * Credits to https://c-for-dummies.com/blog/?p=1112
 * for getline starter code.
 *
*/

/* Launch given child process if possible, or print error message. */
static int launch_child(char* child) {
	if (fork() == 0) {
		execve(child[0], child, NULL);
		exit(1);
	}
	else {
		wait(NULL);
	}
	return 0;	
}

/* Driver for mini shell program. */
int main(int argc, char **argv) {
	
	// welcome message
	printf("Welcome to mini-shell.\n"); 

	char command[MAX_CMDLEN];
	char *cmd = command; 
	command = (char*)malloc(MAX_CMDLEN * sizeof(char));

	// get next line of command
	
	// parse and execute commands until command = 'exit' or 'ctrl+d'
	while (strcmp(&cmd, "exit\n") != 0) {

		// tokenize and launch given command
		launch_child(tokenize(cmd));

		// get next command
		getline(&cmd, &cmd_max_size, stdin);
	}
	
	// exit messsage
	printf("Bye bye.\n");
	return 0;
}
