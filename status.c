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
    status->signaled = false;
    status->pid = getpid();
    status->name = malloc(sizeof(char) * 2048);
    strcpy(status->name, "smallsh");

    return status;
}

struct status* update_status(struct status* status, int code, bool exited, bool signaled, pid_t pid, char* name)
{
    status->code = code;
    status->exited = exited;
    status->signaled = signaled;
    status->pid = pid;
    free(status->name);
    status->name = malloc(sizeof(char) * 2048);
    strcpy(status->name, name);

    return status;
}