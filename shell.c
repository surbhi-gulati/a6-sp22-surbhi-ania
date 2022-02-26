#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"

/* Driver for mini shell program. */
int main(int argc, char **argv) {

	// welcome user and kick off requested child processes
	printf("Welcome to mini-shell.\n"); // welcome message

	// get commands continuously until exit via ctrl+D or exit cmd
	while (1) {
		printf("shell $ ");
		char command[MAX_CMDLEN];
		char* input = fgets(command, MAX_CMDLEN, stdin);
		build_cmd(input);
	}

}
