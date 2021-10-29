// global signal / exit storage for last exited foreground process
#ifndef STATUS_H
#define STATUS_H

#include <sys/types.h>

struct status {
    pid_t pid;
    char* name;
    bool exited;
    bool signaled;
    int code;
};
struct status* init_status();

struct status* update_status(struct status* status, int code, bool exited, bool signaled, pid_t pid, char* name);

#endif