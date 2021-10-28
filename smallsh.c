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

#include "input.h"
#include "parser.h"
#include "smallsh.h"

int main(void)
{
    char* user_input;
    struct command* command;

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
            fflush(stdout);
            exit(1);
            break;

        case 0: // child
            if (DEBUG) {
                printf("(CHILD %d) parent has PID %d\n", getpid(), getppid());
                printf("(CHILD %d) executing command %s with args: ", getpid(), *(command->argv));

                int i = 1;
                while (*(command->argv + i) != NULL) {
                    printf("%s ", *(command->argv + i));
                    i += 1;
                }
                printf("\n");

                fflush(stdout);
            }

            // redirect stdin
            if (command->f_stdin != NULL) {
                int fd = open(command->f_stdin, O_RDONLY);
                if (fd == -1) {
                    printf("(CHILD %d) could not redirect stdin to %s ", getpid(), command->f_stdin);
                    fflush(stdout);
                    perror("(open)");
                    exit(1);
                }

                if (dup2(fd, 0) == -1) {
                    printf("(CHILD %d) could not redirect stdin to %s ", getpid(), command->f_stdin);
                    fflush(stdout);
                    perror("(dup2)");
                    exit(2);
                }
            }

            // redirect stdout
            if (command->f_stdout != NULL) {
                int fd = open(command->f_stdout, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (fd == -1) {
                    printf("(CHILD %d) could not redirect stdout to %s ", getpid(), command->f_stdin);
                    fflush(stdout);
                    perror("(open)");
                    exit(1);
                }

                if (dup2(fd, 1) == -1) {
                    printf("(CHILD %d) could not redirect stdout to %s ", getpid(), command->f_stdin);
                    fflush(stdout);
                    perror("(dup2)");
                    exit(2);
                }
            }

            // execvp will never return if no error
            if (execvp(*command->argv, command->argv) == -1) {
                printf("(CHILD %d) could not execute %s ", getpid(), *(command->argv));
                fflush(stdout);
                perror("(execvp)");
                exit(1); // be sure to exit if command failed! otherwise stuck in child loop
            };

            break;

        default: // parent

            if (DEBUG) {
                printf("(PARENT %d) waiting for child with PID %d to terminate...\n", getpid(), spawn_pid);
                fflush(stdout);
            }

            pid_t child_pid = waitpid(spawn_pid, &child_status, 0);

            if WIFEXITED (child_status) {
                if (DEBUG) {
                    printf("(PARENT %d) child with PID %d exited with status %d\n", getpid(), child_pid, WEXITSTATUS(child_status));
                    fflush(stdout);
                }
            }

            if WIFSIGNALED (child_status) {
                if (DEBUG) {
                    printf("(PARENT %d) child with PID %d exited abnormally with signal %d\n", getpid(), child_pid, WTERMSIG(child_status));
                    fflush(stdout);
                }
            }

            break;
        }
    }
    return 0;
}