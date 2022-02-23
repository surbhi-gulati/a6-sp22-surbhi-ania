#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"

/* Launch given child process if possible, or print error message. */
static int launch_child(char* child) {
	if (fork() == 0) {
		execve(child[0], child, NULL);
		exit(1);
	}
	else {
		wait(NULL);
	}
	return 0;	
}

/* Prompt user for command and retrieve their input in to cmd. */
void get_command(char* cmd) {
	memset(cmd, '\0', MAX_CMDLEN);
	printf("shell $");

	const size_t cmd_max_size = MAX_CMDLEN;	
	getline(&cmd, &cmd_max_size, stdin);
}

/* Check if we should exit shell (1 = True, 0 = False). */
int is_exit_cmd(char* cmd) {
	if (strcmp(&cmd, "exit\n") == 0) {
		return 1;
	}
	return 0;
}

/* Driver for mini shell program. */
int main(int argc, char **argv) {

	// welcome user and kick off requested child processes	
	printf("Welcome to mini-shell.\n"); // welcome message 
	char *cmd = malloc(MAX_CMDLEN * sizeof(char)); // allocate cmd space
	get_command(cmd); // get first shell cmd

	// parse and execute commands until command = 'exit' or 'ctrl+d'
	while (is_exit_cmd(cmd) != 1) {
		launch_child(tokenize(cmd)); // tokenize + launch this cmd
		get_command(cmd); // get next cmd
	}
	
	// free cmd memory & exit with bye message
	free(cmd);
	printf("Bye bye.\n");
	return 0;
}
