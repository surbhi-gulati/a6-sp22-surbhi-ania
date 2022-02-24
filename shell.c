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

	// get cmd's continuously until exit via ctrl+D or exit cmd
	while (1) {

		// get next cmd
		printf("shell $ ");
		char cmd[MAX_CMDLEN];
		char* flag = fgets(cmd, MAX_CMDLEN, stdin); // copy given string argument into expr
		
		// ctrl + D command: exit shell
		if (flag == NULL) {
			printf("\n");
			break;
		}

		// tokenize and put command args in to a string array
		cmd[strcspn(cmd, "\n")] = 0;
		vect_t *command = tokenize(cmd);
		char* cmd_array[vect_size(command) + 1];
		for (int i = 0; i < vect_size(command); i++) {
                	cmd_array[i] = vect_get_copy(command, i);
        	}
        	cmd_array[vect_size(command)] = NULL;

		// exit command: exit shell
		if (strcmp(cmd_array[0], "exit") == 0) {
			break;
		} 

		// process cmd if it is not an exit command
		else {
			if (fork() == 0) {
         			if (execvp(cmd_array[0], cmd_array) < 0) {
                       			printf("No such file or directory\n");
                		}
        		}
        		else {  
               			wait(NULL);
        		}
		}
	}

	// exit message
	printf("Bye bye.\n");
	return 0;
}
