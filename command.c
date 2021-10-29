#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "command.h"

/* function init_empty_command
initializes a new command struct by allocating memory for the struct itself as
well as the argv array. other non-pointer members are initialized.

returns: pointer to an empty command struct with memory allocated.
 */
struct command* init_empty_command()
{

    struct command* command;
    command = malloc(sizeof(struct command));
    if (command == NULL) {
        printf("(init_empty_command) error allocating memory for command...");
        return NULL;
    }

    command->argv = calloc(513, sizeof(char*)); // save +1 space for a NULL termination of array
    if (command->argv == NULL) {
        printf("(init_empty_command) error allocating memory for command->argv...");
        return NULL;
    }

    command->argc = 0;

    command->f_stdin = NULL;
    command->f_stdout = NULL;
    command->f_stderr = NULL;

    command->background = false;

    return command;
}