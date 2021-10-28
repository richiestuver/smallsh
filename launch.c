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
#include "parser.h"

#define DEBUG_LAUNCH true

/* function launch
accepts a fully filled out command struct and executes the command with provided arguments
and values for stdin, stdout, and stderr. If background is true, command will be run in the background.

command: must be a fully filled command struct.
*/
void launch(struct command* command)
{
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
        if (DEBUG_LAUNCH) {
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

        if (DEBUG_LAUNCH) {
            printf("(PARENT %d) waiting for child with PID %d to terminate...\n", getpid(), spawn_pid);
            fflush(stdout);
        }

        pid_t child_pid = waitpid(spawn_pid, &child_status, 0);

        if WIFEXITED (child_status) {
            if (DEBUG_LAUNCH) {
                printf("(PARENT %d) child with PID %d exited with status %d\n", getpid(), child_pid, WEXITSTATUS(child_status));
                fflush(stdout);
            }
        }

        if WIFSIGNALED (child_status) {
            if (DEBUG_LAUNCH) {
                printf("(PARENT %d) child with PID %d exited abnormally with signal %d\n", getpid(), child_pid, WTERMSIG(child_status));
                fflush(stdout);
            }
        }

        break;
    }
}
