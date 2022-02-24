#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"

/* Launch given child process if possible, or print error message. */
static int launch_child(char** child) {
	if (fork() == 0) {
		if(execvp(child[0], child) < 0) {
			printf("No such file or directory\n");
		}
		exit(1);
	}
	else {
		wait(NULL);
	}
	return 0;	
}

char** vect_to_str(vect_t* vect) {
	char* result[vect_size(vect)];
	for (int i = 0; i < vect_size(vect); i++) {
		result[i] = vect_get_copy(vect, i);
	}
	char **res = result;
	return res;
}

/* Driver for mini shell program. */
int main(int argc, char **argv) {

	// welcome user and kick off requested child processes
	printf("Welcome to mini-shell.\n"); // welcome message

	while(1) {
		printf("shell$ ");
		char cmd[MAX_CMDLEN];
		fgets(cmd, MAX_CMDLEN, stdin); // copy given string argument into expr
		if (cmd == NULL) {
			break;
		}
		cmd[strcspn(cmd, "\n")] = 0;

		vect_t *command = tokenize(cmd);
		char** cmd_array = vect_to_str(command);

		if (strcmp(cmd_array[0], "exit") == 1) {
			break;
		} else {
			launch_child(cmd_array);
			memset(cmd_array, '\0', MAX_CMDLEN);
		}
	}
	printf("Bye bye.");
	return 0;
}
