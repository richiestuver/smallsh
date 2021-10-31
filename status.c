#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "status.h"

/* function init_status
initializes storage and sets default values for a new
status struct to be used to store latest information about
last exited foreground process.

returns: pointer to initialized default status struct
*/
struct status* init_status()
{

    struct status* status;
    status = malloc(sizeof(struct status));
    status->code = 0;
    status->exited = true;
    status->signaled = !status->exited;
    status->pid = getpid();
    status->name = malloc(sizeof(char) * 2048);
    strcpy(status->name, "smallsh");

    return status;
}

/* function update_status
updates the provided status struct with new data.

status: pointer to status struct to update
pid: pid_t representing last foreground process
name: command name for last foreground process
code: status signal or exit code of last foreground process
exit_type: EXIT or SIGNAL
*/
struct status* update_status(struct status* status, pid_t pid, char* name, int code, enum EXIT_TYPE exit_type)
{

    status->code = code;
    status->exited = (exit_type == EXIT) ? true : false;
    status->signaled = !status->exited;
    status->pid = pid;
    free(status->name);
    status->name = malloc(sizeof(char) * 2048);
    strcpy(status->name, name);

    return status;
}