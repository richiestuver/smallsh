/*

Title: Assignment 3: smallsh
Author: Richie Stuver
Course: CS344 - Operating Systems I
Created: 10-20-21

*/

#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtins.h"
#include "command.h"
#include "input.h"
#include "launch.h"
#include "parser.h"
#include "sighandlers.h"
#include "smallsh.h"
#include "status.h"

int main(void)
{

    char* user_input;
    struct status* stat = NULL;
    struct command* command = NULL;

    stat = init_status();
    register_SIGINT(SIG_IGN);
    register_SIGCHLD(cleanup_children);

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

        char* cmd;
        if ((cmd = check_builtins(*(command->argv))) != NULL) {
            if (DEBUG) {
                printf("((main %d) found builtin cmd %s\n", getpid(), cmd);
            }

            launch_builtin(command, stat);
        }

        else {
            launch(command, stat);
        }
    }
    return 0;
}