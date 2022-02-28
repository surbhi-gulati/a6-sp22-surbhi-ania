#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"

/* Update prev_cmd to contain the contents of the current command. */
static void update_prev(char** cmd, char** prev_cmd) {
        for (int i = 0; i < MAX_CMDLEN; i++) {
                prev_cmd[i] = cmd[i];
        }
}

/* Process given child program on foreground. */
static void exec_child(char** cmd, char** prev_cmd) {
	if (fork() == 0) {
                if (execvp(cmd[0], cmd) < 0) {
			printf("No such file or directory\n");
        	}
        }
	else {
		wait(NULL);
		update_prev(cmd, prev_cmd);
        }
}

/* Help message for malformed built-in commands. */
static void builtin_malformed(char* cmd, int expt_args) {
	printf("Malformed input. Command: \'%s\' expected %d arguments.\nEnter \'help\' to view documentation.", cmd, expt_args);
} 

/* Style help documentation: bold command and provide plaintext additional information. */
static void builtin_help(char* cmd, char* documentation) {
	printf("\'%s\':\t%s", cmd, documentation);
}

/* Tokenize and put STDIN command arguments in to command string array. */
static void build_cmd(char* input, char** command) {
	input[strcspn(input, "\n")] = 0;
	vect_t* cmd_vect = tokenize(input);
	for (int i = 0; i < vect_size(cmd_vect); i++) {
		command[i] = vect_get_copy(cmd_vect, i);
	}
	command[vect_size(cmd_vect)] = NULL;
	vect_delete(cmd_vect);
}

/* Clear memory consumed by current and previous commands and exit program. */
static void quit_shell(char** command, char** prev_command) {
	for (int i = 0; i < MAX_CMDLEN; i++) {
		free(command[i]);
		free(prev_command[i]);
	}	
	printf("REACHED QUIT SHELL\n");
	exit(0);
}

/* Changes the current working directory of the shell  */
static void change_dir(char** cmd) {
        char* dir = (char *) malloc(MAX_CMDLEN * sizeof(char));
        strncpy(dir, cmd[1], strlen(cmd[1]));

        // change directory
        int newDir = chdir(dir);
        free(dir);
        if (newDir < 0) {
                printf("Error occurred while changing directory.\n");
        }
}

/* Count number of arguments in the given command array. */
static int cmd_arguments(char** cmd) {
	int args = 0;
	while (cmd[args] != NULL) {
		args++;
	}
	return args;
}

/* Check whether the tokenized command array is a built-in command, 
 * if it is then complete custom execution. 
 * Otherwise tokenize and execute the child process. */
static void exec_proc(char** cmd, char** prev_cmd) {

	// length of command array for command validation
	int cmd_args = cmd_arguments(cmd);

	// if prev requested: print + execute previous command
	if (strcmp(cmd[0], "prev") == 0) {
		if (cmd_args != 1) {
			builtin_malformed("prev", 1);
		}
		else {				
			exec_child(prev_cmd, prev_cmd);
			update_prev(cmd, prev_cmd);
		}
	}

	// if help requested: print help info        	
	else if (strcmp(cmd[0], "help") == 0) {
		builtin_help("exit / ctrl+d", "quit shell");
		builtin_help("cd", "change directory");
		builtin_help("source", "execute commands in file, line by line");
		builtin_help("prev", "print and execute previous command");
		builtin_help("help", "view documentation for mini-shell builtin commands");
		update_prev(cmd, prev_cmd);
	}

	// if cd requested: change current working directory of shell
	else if (strcmp(cmd[0], "cd") == 0) {
		if (cmd_args != 2) {
			builtin_malformed("cd", 2);
		}
		else {
			change_dir(cmd);
			update_prev(cmd, prev_cmd);
		}
	}

	// if source requested: get filename and execute commands line by line
	else if (strncmp(cmd[0], "source", 6) == 0) {
		if (cmd_args != 2) {
			builtin_malformed("source", 2);
		}
		else {
			char line[MAX_STRLEN];
			FILE* source_read = fopen(cmd[1], "r");
			// read lines from sourcefile and execute one by one until EOF
			while (fgets(line, sizeof(line), source_read) != NULL) {
				char new_cmd[MAX_STRLEN];
				// exec_proc(build_cmd(new_cmd), build_cmd(prev_cmd));
			}
			fclose(source_read);
		}
	}

	else {
		exec_child(cmd, prev_cmd);
	}

}
