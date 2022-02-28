#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"
#include "shell_func.c"

/* Driver for mini shell program. */
int main(int argc, char **argv) {

	// welcome user and kick off requested child processes
	printf("Welcome to mini-shell.\n"); // welcome message

	// save current and previous commands in memory
	char input[MAX_CMDLEN];
        char* command[MAX_CMDLEN];	
	char* prev_command[MAX_CMDLEN];

	// get commands continuously until exit via ctrl+D or exit cmd
	while (1) {
		printf("shell $ ");
		char* flag = fgets(input, MAX_CMDLEN, stdin);

		if (flag == NULL) {
			printf("\nBye bye.\n");
			break;
		}
		build_cmd(input, command);
		exec_proc(command, prev_command);
	}
	
	return 0;
}
