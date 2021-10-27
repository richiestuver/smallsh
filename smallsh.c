/*

Title: Assignment 3: smallsh
Author: Richie Stuver
Course: CS344 - Operating Systems I
Created: 10-20-21

*/

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "input.h"
#include "parser.h"
#include "smallsh.h"

int main(void)
{
    char* user_input;
    struct command* command;
    printf("Hello World!\n");

    while (true) {
        display_prompt();

        user_input = get_user_input();
        user_input = strip_newlines(user_input);
        user_input = strip_comments(user_input);

        if (user_input == NULL) {
            if (DEBUG) {
                printf("(main) user input is NULL...\n");
            }
            continue;
        };

        command = parse(user_input);

        if (DEBUG) {
            printf("(main) fully parsed command struct is: \n");
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

        int child_status;
        pid_t spawn_pid = INT_MIN;
        spawn_pid = fork();

        switch (spawn_pid) {
        case -1:
            printf("(main) fatal error: spawning process failed. exiting...\n");
            exit(1);
            break;

        case 0: // child
            printf("(CHILD %d) parent has PID %d\n", getpid(), getppid());
            printf("(CHILD %d) executing sample command ls \n", getpid());

            char* argv[] = { "ls", NULL };
            if (execvp("ls", argv) == -1) {
                printf("(CHILD %d) could not execute ls\n", getpid());
            };

            break;

        default: // parent
            printf("(PARENT %d) waiting for child with PID %d to terminate...\n", getpid(), spawn_pid);
            pid_t child_pid = waitpid(spawn_pid, &child_status, 0);

            if WIFEXITED (child_status) {
                printf("(PARENT %d) child with PID %d exited with status %d\n", getpid(), child_pid, WEXITSTATUS(child_status));
            }

            if WIFSIGNALED (child_status) {
                printf("(PARENT %d) child with PID %d exited abnormally with signal %d\n", getpid(), child_pid, WTERMSIG(child_status));
            }

            break;
        }
    }
    return 0;
}