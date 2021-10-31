#ifndef LAUNCH_H
#define LAUNCH_H

#include "command.h"
#include "exec_env.h"
#include "status.h"

#define DEBUG_LAUNCH false

/* function launch
accepts a fully filled out command struct and executes the command with provided arguments
and values for stdin, stdout, and stderr. If background is true, command will be run in the background.

command: must be a fully filled command struct.
status: struct to update the command execution status
*/
void launch(struct command* command, struct status* status);
#endif