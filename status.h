// global signal / exit storage for last exited foreground process
#ifndef STATUS_H
#define STATUS_H

#include <sys/types.h>

enum EXIT_TYPE {
    EXIT,
    SIGNAL
};

struct status {
    pid_t pid;
    char* name;
    bool exited;
    bool signaled;
    int code;
};
struct status* init_status();

struct status* update_status(struct status* status, pid_t pid, char* name, int code, enum EXIT_TYPE exit_type);

#endif