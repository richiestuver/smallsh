// contains definition for command struct used throughout shell execution
#ifndef COMMAND_H
#define COMMAND_H

struct command {
    char* cmd;
    int argc;
    char** argv;
    char* f_stdin;
    char* f_stdout;
    char* f_stderr;
    bool background;
};

/* function init_empty_command
initializes a new command struct by allocating memory for the struct itself as
well as the argv array. other non-pointer members are initialized.

returns: pointer to an empty command struct with memory allocated.
 */
struct command* init_empty_command();

#endif