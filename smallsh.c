/*

Title: Assignment 3: smallsh
Author: Richie Stuver
Course: CS344 - Operating Systems I
Created: 10-20-21

*/

#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
#include "input.h"
#include "launch.h"
#include "parser.h"
#include "smallsh.h"

char* BUILTIN_COMMANDS[] = { "exit", "cd", "status" };

/* function exit
exits the shell and terminates all other processes or jobs that the shell has started.
returns: 0
*/

void builtin_exit(void)
{
    if (DEBUG) {
        printf("(builtin_exit %d) terminating shell...\n", getpid());
    }
    exit(0);
}

void builtin_cd(struct command* command)
{

    char* dest = *(command->argv + 1);

    if (dest == NULL) {
        char* HOME = getenv("HOME");
        printf("(builtin_cd %d) changing to HOME dir %s\n", getpid(), HOME);

        dest = HOME;
    }

    if (chdir(dest) != 0) {
        printf("(builtin_cd %d) cannot change to dir %s ", getpid(), dest);
        fflush(stdout);
        perror("(chdir)");
    };

    char* cwd = NULL;
    printf("(builtin_cd %d) %s\n", getpid(), getcwd(cwd, 0));
}

int main(void)
{
    char* user_input;
    struct command* command;

    while (true) {
        display_prompt();

        user_input = strip_comments(strip_newlines(get_user_input()));

        if (user_input == NULL) {
            if (DEBUG) {
                printf("(main %d) user input is NULL...\n", getpid());
                fflush(stdout);
            }
            continue;
        };

        command = parse(user_input);

        if (DEBUG) {
            printf("(main %d) fully parsed command struct is: \n", getpid());
            printf("        command (arg0): %s\n", *(command->argv + 0));
            int i = 1;
            while (*(command->argv + i) != NULL) {
                printf("        arg%d: %s\n", i, *(command->argv + i));
                i += 1;
            }

            printf("        background: %s\n", (command->background == 1) ? "true" : "false");
            printf("        stdin: %s\n", (command->f_stdin));
            printf("        stdout: %s\n", (command->f_stdout));
            printf("        stderr: %s\n", (command->f_stderr));
        }

        // TODO: builtin commands for exit, cd and status

        if (strcmp(*(command->argv), "exit") == 0) {
            printf("(main %d) LAUNCHING exit\n", getpid());
            fflush(stdout);
            builtin_exit();
        } else if (strcmp(*(command->argv), "cd") == 0) {
            printf("(main %d) LAUNCHING cd\n", getpid());
            fflush(stdout);
            builtin_cd(command);
        } else {
            launch(command);
        }
    }
    return 0;
}