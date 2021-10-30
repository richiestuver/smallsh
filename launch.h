#ifndef LAUNCH_H
#define LAUNCH_H

#include "command.h"
#include "exec_env.h"
#include "status.h"

#define DEBUG_LAUNCH true

void launch(struct command* command, struct status* status, struct exec_env* exec_env);
#endif