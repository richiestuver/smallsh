#include <stdbool.h>
#include <stdlib.h>

#include "exec_env.h"

struct exec_env* init_exec_env(void)
{
    struct exec_env* exec_env = NULL;
    exec_env = malloc(sizeof(struct exec_env));
    exec_env->background_enabled = true;
    return exec_env;
}

struct exec_env* exec_env = NULL;