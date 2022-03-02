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
	char prev_input[MAX_CMDLEN];
        char* command[MAX_CMDLEN];	

	// get commands continuously until exit via ctrl+D or exit cmd
	while (1) {
		printf("shell $ ");
		char* flag = fgets(input, MAX_CMDLEN, stdin);

		// exit if ctrl+D
		if (flag == NULL) {
			printf("\n");
			break;
		}

		// build command for execution
		build_cmd(input, command);
		
		// exit if command prompts exit
		if (strcmp(command[0], "exit") == 0) {
			break;
		}

		// execute given command
		exec_proc(command, input, prev_input);
	}

	// free memory, print closing message, and exit program	
	quit_shell(command);	
	return 0;
}
