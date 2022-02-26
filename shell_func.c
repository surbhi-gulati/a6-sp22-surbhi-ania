#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"

/* Process given child program on foreground. */
static void exec_child(char** cmd_array) {
	if (fork() == 0) {
                if (execvp(cmd_array[0], cmd_array) < 0) {
			printf("No such file or directory\n");
        	}
        }
	else {
		wait(NULL);
        }
}

/* Help message for malformed built-in commands. */
static void builtin_malformed(char* cmd, int expt_args) {
	printf("Malformed input. Command: '%s' expected %d arguments.
			\nEnter 'help' to view documentation.", cmd, expt_args);
} 

/* Style help documentation: bold command and provide plaintext additional information. */
static void builtin_help(char* cmd, char* documentation) {
	printf("'%b':\t%s", cmd, documentation);
}


/* Check whether the command is a built-in command, if it is then complete custom execution. 
   Otherwise tokenize and execute the child process. */
static void built_in_proc(char* cmd, char* prev_cmd) {

	// commmand length for validating inputs before attempting execution
	int cmd_len = sizeof(cmd) / sizeof(*cmd);

        // if ctrl+d or exit requested: quit shell
	if (strcmp(cmd[0], NULL) == 0 || strcmp(cmd[1], "exit") == 0) {
		printf("Bye bye.\n");
		exit(0); // TODO: ensure this doesn't cause mem leaks
	}

	// if prev requested: print + execute previous command
	else if (strcmp(cmd[0], "prev") == 0) {
		if (cmd_len != 1) {
			builtin_malformed("prev", 1);
		}
		else {				
			exec_child(tokenize_cmd(prev_cmd));
		}
	}

	// if help requested: print help info        	
	else if (strcmp(cmd, "help")) {
		builtin_help("exit / ctrl+d", "quit shell");
		builtin_help("cd", "change directory");
		builtin_help("source", "execute commands in file, line by line");
		builtin_help("prev", "print and execute previous command");
		builtin_help("help", "view documentation for mini-shell builtin commands");
	}

	// if cd requested: change current working directory of shell
	else if (strcmp(cmd[0], "cd") == 0) {
		if (cmd_len != 2) {
			builtin_malformed("cd", 2);
		}
		else {
			//
		}
	}

	// if source requested: get filename and execute commands line by line
	else if (strncmp(cmd[0], "source", 6) == 0) {
		if (cmd_len != 2) {
			builtin_malformed("source", 2);
		}
		else {
			// read -> save cmd, prev -> exec until cmd = eof
		}
	}

	else {
		// exec_child(cmd);
	}

}
