#ifndef LAUNCH_H
#define LAUNCH_H

#include "command.h"
#include "exec_env.h"
#include "status.h"

#define DEBUG_LAUNCH false

void launch(struct command* command, struct status* status);
#endif