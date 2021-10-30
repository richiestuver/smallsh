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
#include "launch.h"
#include "sighandlers.h"
#include "status.h"

/* function launch
accepts a fully filled out command struct and executes the command with provided arguments
and values for stdin, stdout, and stderr. If background is true, command will be run in the background.

command: must be a fully filled command struct.
status: struct to update the command execution status
*/
void launch(struct command* command, struct status* status)
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

        if (!(command->background)) {
            register_SIGINT(SIG_DFL);
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

        if (DEBUG_LAUNCH) {
            printf("(CHILD %d) executing command %s with args: ", getpid(), *(command->argv));

            int i = 1;
            while (*(command->argv + i) != NULL) {
                printf("%s ", *(command->argv + i));
                i += 1;
            }
            printf("\n");

            fflush(stdout);
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

        if (!(command->background)) {
            if (DEBUG_LAUNCH) {
                printf("(PARENT %d) waiting for child with PID %d to terminate...\n", getpid(), spawn_pid);
                fflush(stdout);
            }

            pid_t child_pid = waitpid(spawn_pid, &child_status, 0);

            if (child_pid == -1) {
                perror("(waitpqid)");
                child_pid = spawn_pid;
                fflush(stdout);
            }

            if WIFEXITED (child_status) {
                if (DEBUG_LAUNCH) {
                    printf("(PARENT %d) child with PID %d exited with status %d\n", getpid(), child_pid, WEXITSTATUS(child_status));
                    fflush(stdout);
                }

                status = update_status(status, child_pid, *command->argv, WEXITSTATUS(child_status), EXIT);

                if (DEBUG_LAUNCH) {
                    printf("(PARENT %d) code: %d \n", getpid(), status->code);
                    printf("(PARENT %d) pid: %d \n", getpid(), status->pid);
                    printf("(PARENT %d) name: %s \n", getpid(), status->name);
                    printf("(PARENT %d) exited: %s \n", getpid(), (status->exited) == 1 ? "true" : "false");
                    printf("(PARENT %d) signaled: %s \n", getpid(), (status->signaled) == 1 ? "true" : "false");
                    fflush(stdout);
                }
            }

            if WIFSIGNALED (child_status) {

                printf("terminated by signal %d\n", WTERMSIG(child_status));
                fflush(stdout);

                status = update_status(status, child_pid, *command->argv, WTERMSIG(child_status), SIGNAL);

                if (DEBUG_LAUNCH) {
                    printf("(PARENT %d) code: %d \n", getpid(), status->code);
                    printf("(PARENT %d) pid: %d \n", getpid(), status->pid);
                    printf("(PARENT %d) name: %s \n", getpid(), status->name);
                    printf("(PARENT %d) exited: %s \n", getpid(), (status->exited) == 1 ? "true" : "false");
                    printf("(PARENT %d) signaled: %s \n", getpid(), (status->signaled) == 1 ? "true" : "false");
                    fflush(stdout);
                }
            }
        } else {
            printf("background pid is %d\n", spawn_pid);
            fflush(stdout);
        }

        break;
    }
}
