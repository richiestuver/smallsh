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

/* function init_status
initializes storage and sets default values for a new
status struct to be used to store latest information about
last exited foreground process.

returns: pointer to initialized default status struct
*/
struct status* init_status();

/* function update_status
updates the provided status struct with new data.

status: pointer to status struct to update
pid: pid_t representing last foreground process
name: command name for last foreground process
code: status signal or exit code of last foreground process
exit_type: EXIT or SIGNAL
*/
struct status* update_status(struct status* status, pid_t pid, char* name, int code, enum EXIT_TYPE exit_type);

#endif