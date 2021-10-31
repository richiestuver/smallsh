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

/* function check_builtins
receives a pointer to char array representing a target command.
this command is checked against and the list of builtin commands
and if an exact match, the matching builtin is returned or NULL
if no match.

cmd: char arr representing command to be checked
returns: char * to matching builtin or NULL
*/
char* check_builtins(char* cmd);

/* function launch_builtin
is a helper function to execute a builtin-variant of
a provided command (in the form of pointer to command struct).
a matching builtin command is executed. function does not return.
*/
void launch_builtin(struct command* command, struct status* status);

#endif