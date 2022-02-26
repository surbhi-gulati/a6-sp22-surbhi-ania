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

/* Tokenize and put STDIN command arguments in to a string array. */
static char* build_cmd(char input) {
	input[strcpsn(input, "\n")] = 0;
	vect_t* cmd_vect = tokenize(input);
	char* command[vect_size(cmd_vect) + 1];
	for (int i = 0; i < vect_size(cmd_vect); i++) {
		command[i] = vect_get_copy(cmd_vect, i);
	}
	command[vect_size(cmd_vect)] = NULL;
	vect_delete(cmd_vect);
	return command;
}

/* Clear memory consumed by current and previous commands and exit program. */
static void quit_shell(int cmd_args, char* cmd, int prev_args, char* prev_cmd) {
	for (int i = 0; i < cmd_args; i++) {
		free(cmd[i]);
	}
	for (int i = 0; i < prev_args; i++) {
		free(prev_cmd[i]);
	}
	exit(0);
}

/* Check whether the tokenized command array is a built-in command, 
 * if it is then complete custom execution. 
 * Otherwise tokenize and execute the child process. */
static void builtin_proc(int cmd_args, char* cmd, int prev_args, char* prev_cmd) {

        // if ctrl+d requested: go to next line & quit shell
	if (strcmp(cmd[0], NULL) == 0) {
		printf("\nBye bye.\n");
		quit_shell(cmd_args, cmd, prev_args, prev_cmd);
	}

	// if ctrl+d requested: quit shell
	else if (strcmp(cmd[1], "exit") == 0) {
		printf("Bye bye.\n");
		quit_shell(cmd_args, cmd, prev_args, prev_cmd);
	}

	// if prev requested: print + execute previous command
	else if (strcmp(cmd[0], "prev") == 0) {
		if (cmd_args != 1) {
			builtin_malformed("prev", 1);
		}
		else {				
			exec_child(prev_cmd);
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
		if (cmd_args != 2) {
			builtin_malformed("cd", 2);
		}
		else {
			//
		}
	}

	// if source requested: get filename and execute commands line by line
	else if (strncmp(cmd[0], "source", 6) == 0) {
		if (cmd_args != 2) {
			builtin_malformed("source", 2);
		}
		else {
			// read -> save cmd, prev -> exec until cmd = eof
		}
	}

	else {
		exec_child(cmd);
	}

}
