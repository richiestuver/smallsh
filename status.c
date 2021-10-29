#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "status.h"

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