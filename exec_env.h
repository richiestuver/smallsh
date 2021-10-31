#ifndef EXEC_ENV_H
#define EXEC_ENV_H

/* exec_env declaration provides global
availability across smallsh program to share and
update the same execution environment.
declare with extern to access in other files.
*/
struct exec_env {
    bool background_enabled;
};

/* function init_exec_env
allocates storage for a new execution environment
and returns a pointer to this struct. By default
background processes are enabled. Other global settings
pertaining to command execution may be stored here.
*/
struct exec_env* init_exec_env(void);

#endif