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
		execve(child[0], child, NULL);
		exit(1);
	}
	else {
		wait(NULL);
	}
	return 0;	
}

/* Check if we should exit shell (1 = True, 0 = False). */
int is_exit_cmd(char* cmd) {
	if (strcmp(cmd, "exit") == 0) {
		return 1;
	}
	return 0;
}

char* vect_to_str(vect_t* vect) {
	char result[vect_size(vect)];
	for (int i = 0; i < vect_size(vect); i++) {
		result[i] = *vect_get_copy(vect, i);
	}
	char* res = result;
	return res;
}

/* Driver for mini shell program. */
int main(int argc, char **argv) {

	// welcome user and kick off requested child processes
	printf("Welcome to mini-shell.\n"); // welcome message

	while(1) {
		printf("shell$ ");
		char cmd[MAX_CMDLEN];
		fgets(cmd, MAX_CMDLEN, stdin); // copy given string argument in to expr
		cmd[strcspn(cmd, "\n")] = 0;

		vect_t *command = tokenize(cmd);
		char* cmd_array = vect_to_str(command);

		if (is_exit_cmd(cmd_array[0]) == 1) {
			printf("Bye bye.\n");
			return 0;
		} else {
			launch_child(cmd_array);
		}
	}
}
