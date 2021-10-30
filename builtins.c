#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtins.h"
#include "command.h"
#include "status.h"

char* BUILTIN_COMMANDS[] = { "exit", "cd", "status", NULL };

/* function exit
exits the shell and terminates all other processes or jobs that the shell has started.
returns: 0
*/
void builtin_exit(void)
{
    if (DEBUG_BUILTINS) {
        printf("(builtin_exit %d) terminating shell...\n", getpid());
    }
    exit(0);
}

/* function builtin_cd
receives a struct command which contains an optional argument for
destination directory. if argument not provided, cd defaults to
changing to the HOME directory provided in environment.
*/
void builtin_cd(struct command* command)
{

    char* dest = *(command->argv + 1);

    if (dest == NULL) {
        char* HOME = getenv("HOME");
        dest = HOME;

        if (dest == NULL) {
            printf("(builtin_cd %d) dir not changed: no environment variable found for HOME\n", getpid());
            fflush(stdout);
            return;
        }
    }

    if (chdir(dest) != 0) {
        printf("(builtin_cd %d) cannot change to dir %s ", getpid(), dest);
        fflush(stdout);
        perror("(chdir)");
    };

    if (DEBUG_BUILTINS) {
        char* cwd = NULL;
        printf("(builtin_cd %d) %s\n", getpid(), getcwd(cwd, 0));
    }
    return;
}

/* function builtin_status
retrieves the exit type and code from the global
status struct and reports this value out to the terminal.

status: pointer to initialized status struct
*/
void builtin_status(struct status* status)
{
    if (DEBUG_BUILTINS) {

        printf("(PARENT %d) code: %d \n", getpid(), status->code);
        printf("(PARENT %d) pid: %d \n", getpid(), status->pid);
        printf("(PARENT %d) name: %s \n", getpid(), status->name);
        printf("(PARENT %d) exited: %s \n", getpid(), (status->exited) == 1 ? "true" : "false");
        printf("(PARENT %d) signaled: %s \n", getpid(), (status->signaled) == 1 ? "true" : "false");
        fflush(stdout);
    }

    if (status->exited) {
        printf("exit value %d\n", status->code);
    }

    else { // status->signaled == true
        printf("terminated by signal %d\n", status->code);
    }

    fflush(stdout);
    return;
}

char* check_builtins(char* cmd)
{
    int i = 0;
    while (*(BUILTIN_COMMANDS + i) != NULL) {
        if (strcmp(cmd, *(BUILTIN_COMMANDS + i)) == 0) {
            return *(BUILTIN_COMMANDS + i);
        }
        i++;
    }
    return NULL;
}

void launch_builtin(struct command* command, struct status* status)
{

    int i = 0;
    while (*(BUILTIN_COMMANDS + i) != NULL) {
        if (strcmp(*(command->argv), *(BUILTIN_COMMANDS + i)) == 0) {
            break;
        }
        i++;
    }

    switch (i) {
    case 0:
        builtin_exit();
        break;

    case 1:
        builtin_cd(command);
        break;

    case 2:
        builtin_status(status);
        break;

    default:
        printf("(launch_builtin %d) there was an error trying to launch %s", getpid(), *(command->argv));
        break;
    }
}