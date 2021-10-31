#include <stdbool.h>
#include <stdlib.h>

#include "exec_env.h"

/* function init_exec_env
allocates storage for a new execution environment
and returns a pointer to this struct. By default
background processes are enabled. Other global settings
pertaining to command execution may be stored here.
*/
struct exec_env* init_exec_env(void)
{
    struct exec_env* exec_env = NULL;
    exec_env = malloc(sizeof(struct exec_env));
    exec_env->background_enabled = true;
    return exec_env;
}

/* exec_env declaration provides global
availability across smallsh program to share and
update the same execution environment.
declare with extern to access in other files.
*/
struct exec_env* exec_env = NULL;