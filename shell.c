#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"

/* Launch given child process if possible, or print error message. */
static void launch_child(char** child) {
	if (fork() == 0) {
		if (execvp(child[0], child) < 0) {
			printf("No such file or directory\n");
		}
		exit(1);
	}
	else {
		wait(NULL);
	}
	// return 0;	
}

/* Prepend '/bin/' to command if not present so that system can find and 
execute the given command. */
static char** sys_command(char** cmd) {
        if (strncmp(cmd[0], "/bin/", 5) != 0) {
                char *bin_append = calloc(MAX_CMDLEN, sizeof(char));
                memcpy(bin_append, "/bin/", 5);

                strncat(bin_append, cmd[0], strlen(cmd[0]));
                cmd[0] = bin_append;
                free(bin_append);
        }
        return cmd;
}

/* Convert the given vector to a string and return a pointer to it. */
static char** vect_to_str(vect_t* vect) {
	char* result[vect_size(vect) + 1];
	for (int i = 0; i < vect_size(vect); i++) {
		result[i] = vect_get_copy(vect, i);
	}
	result[vect_size(vect)] = NULL;
	char **res = result;
	return sys_command(res);
}

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
		char** cmd_array = vect_to_str(command);

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
