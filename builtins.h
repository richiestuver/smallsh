#ifndef BUILTINS_H
#define BUILTINS_H

#include "command.h"
#include "status.h"

#define DEBUG_BUILTINS false

/* function exit
exits the shell and terminates all other processes or jobs that the shell has started.
returns: 0
*/
void builtin_exit(void);

/* function builtin_cd
receives a struct command which contains an optional argument for
destination directory. if argument not provided, cd defaults to
changing to the HOME directory provided in environment.
*/
void builtin_cd(struct command* command);

/* function builtin_status
retrieves the exit type and code from the global
status struct and reports this value out to the terminal.

status: pointer to initialized status struct
*/
void builtin_status(struct status* status);

char* check_builtins(char* cmd);

void launch_builtin(struct command* command, struct status* status);

#endif