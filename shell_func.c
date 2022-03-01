#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"

/* Count number of arguments in the given command array. */
static int cmd_arguments(char** cmd) {
        int args = 0;
        while (cmd[args] != NULL) {
                args++;
        }
        return args;
}

/* Update prev_cmd to contain the contents of the current command. */
static void update_prev(char** cmd, char** prev_cmd, char* input, char* prev_input) {
        for (int i = 0; i < MAX_CMDLEN; i++) {
                // strncpy(prev_cmd[i], cmd[i], strlen(cmd[i]));
		// strncpy(prev_input[i], input[i], strlen(input[i]));
		prev_cmd[i] = cmd[i];
		prev_input[i] = input[i];
        }
}

/* Process given child program on foreground. */
static void exec_child(char** cmd, char** prev_cmd, char* input, char* prev_input) {
	if (fork() == 0) {
                if (execvp(cmd[0], cmd) < 0) {
			printf("No such file or directory\n");
        	}
        }
	else {
		wait(NULL);
		update_prev(cmd, prev_cmd, input, prev_input);
        }
}

/* Help message for malformed built-in commands. */
static void builtin_malformed(char* cmd, int expt_args) {
	printf("Malformed input. Command: \'%s\' expected %d arguments.\n", cmd, expt_args);
	printf("Enter \'help\' to view documentation.\n");
} 

/* Style help documentation: bold command and provide plaintext additional information. */
static void builtin_help(char* cmd, char* documentation) {
	printf("\'%s\'  :  %s\n", cmd, documentation);
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
	printf("Bye bye.\n");
	int i = 0;
	while (command[i] != NULL) {
		free(command[i]);
		i++;
	}

	i = 0;
	while (prev_command[i] != NULL) {
		free(command[i]);
		i++;
	} 
	for (int i = 0; i < MAX_CMDLEN; i++) {
		// free(command[i]);
		// free(prev_command[i]);
		continue;
	}
	// free(command);
	// free(prev_command);	
	printf("REACHED QUIT SHELL\n");
	exit(0);
}

/* Changes the current working directory of the shell. If no directory is provided
 * (ie. dir_given = 0) then change to home directory.  */
static void change_dir(char** cmd, int dir_given) {
        char* dir = (char *) malloc(MAX_CMDLEN * sizeof(char));
        dir = strncpy(dir, cmd[1], strlen(cmd[1]));
	// change directory
        int newDir = chdir(dir);
	free(dir);
        if (newDir < 0) {
                printf("Error occurred while changing directory.\n");
        }
}

/* Checks whether the command array contains one of the special characters.  */
static int contains_special(char** cmd) {
	int length = cmd_arguments(cmd);
	for (int cmd_i = 0; cmd_i < length; cmd_i++) {
		if (strcmp("<", cmd[cmd_i]) == 0) {
			return 1;
		} else if (strcmp(">", cmd[cmd_i]) == 0) {
			return 2;
		} else if (strcmp(";", cmd[cmd_i]) == 0) {
			return 3;
		} else if (strcmp("|", cmd[cmd_i]) == 0) {
			return 4;
		}
	}
	return 0;
}

/* Splits the array into two arrays, one that is on the left side of the first occurence of the symbol and the other that is on the right sideof the first occurence of the symbol. */
static void split_array(char* element, char** original, char** first, char** second) {
	int i_original = 0;
	int i_first = 0;
	int length = cmd_arguments(original);
	while(strcmp(element, original[i_original]) != 0) {
		first[i_first] = original[i_original];
		i_first++;
		i_original++;
	}
	int i_second = 0;
	i_original++;
	while (i_original < length) {
		second[i_second] = original[i_original];
		i_second++;
		i_original++;
	}
}

/* Redirects input into the left command. */ 
static void redirection_left(char** cmd, char** prev_cmd, char* input, char* prev_input) {}

/* Redirects output of left command into right. */
static void redirection_right(char** cmd, char** prev_cmd, char* input, char* prev_input) {}

/* Sequences a function. */
static void sequence(char** cmd, char** prev_cmd, char* input, char* prev_input) {
	char *first_command[MAX_CMDLEN];
	char *second_command[MAX_CMDLEN];
	split_array(";", cmd, first_command, second_command);
	if (fork() == 0) {
                if (execvp(first_command[0], first_command) < 0) {
                        printf("No such file or directory\n");
                }
        }
        else {
                wait(NULL);
		if (fork() == 0) {
			if (execvp(second_command[0], second_command) < 0) {
                        	printf("No such file or directory\n");
                	}
		} else {
			wait(NULL);
		}
        }        
}

/* Pipes output of left side command as input of the right side command. */
static void pipe_cmd(char** cmd, char** prev_cmd, char* input, char* prev_input) {}

/* Delegates to the correct special token function depending on the output of contains_special.  */
static void delegate_special(char** cmd, char** prev_cmd, char* input, char* prev_input) {
	int special = contains_special(cmd);
	if (special == 1) {
		redirection_left(cmd, prev_cmd, input, prev_input);
	} else if (special == 2) {
		redirection_right(cmd, prev_cmd, input, prev_input);
	} else if (special == 3) {
		sequence(cmd, prev_cmd, input, prev_input);
	} else if (special == 4) {
		pipe_cmd(cmd, prev_cmd, input, prev_input);
	}
}


/* Check whether the tokenized command array is a built-in command, 
 * if it is then complete custom execution. 
 * Otherwise tokenize and execute the child process. */
static void exec_proc(char** cmd, char** prev_cmd, char* input, char* prev_input) {

	// length of command array for command validation
	int cmd_args = cmd_arguments(cmd);

	if (contains_special(cmd) > 0) {
		delegate_special(cmd, prev_cmd, input, prev_input);
		update_prev(cmd, prev_cmd, input, prev_input);
	}
	// if prev requested: print + execute previous command
	else if (strcmp(cmd[0], "prev") == 0) {
		if (cmd_args != 1) {
			builtin_malformed("prev", 1);
		}
		else {
		        printf("%s\n", prev_input);	
			update_prev(prev_cmd, cmd, prev_input, prev_input); // cur cmd = prev cmd			
			exec_proc(prev_cmd, prev_cmd, prev_input, prev_input);
		}
	}

	// if help requested: print help info        	
	else if (strcmp(cmd[0], "help") == 0) {
		builtin_help("exit / ctrl+d", "quit shell");
		builtin_help("cd", "change directory");
		builtin_help("source", "execute commands in file, line by line");
		builtin_help("prev", "print and execute previous command");
		builtin_help("help", "view documentation for mini-shell builtin commands");
		update_prev(cmd, prev_cmd, input, prev_input);
	}

	// if cd requested: change current working directory of shell
	else if (strcmp(cmd[0], "cd") == 0) {
		if (cmd_args > 2) {
        		printf("Malformed input. Command: \'cd\' expected 1-2 arguments.\n");
	        	printf("Enter \'help\' to view documentation.\n");
		} else if (cmd_args == 1) {
			chdir("..");
		}
		else {
			change_dir(cmd, cmd_args);
			update_prev(cmd, prev_cmd, input, prev_input);
		}
	}

	// if source requested: get filename and execute commands line by line
	else if (strcmp(cmd[0], "source") == 0) {
		if (cmd_args != 2) {
			builtin_malformed("source", 2);
		}
		else {
			char line[MAX_STRLEN];
			FILE* source_read = fopen(cmd[1], "r");
			// read lines from sourcefile and execute one by one until EOF
			while (fgets(line, sizeof(line), source_read) != NULL) {
				build_cmd(line, cmd);
				exec_proc(cmd, prev_cmd, input, prev_input);
			}
			fclose(source_read);
		}
	}

	else {
		exec_child(cmd, prev_cmd, input, prev_input);
	}

}
