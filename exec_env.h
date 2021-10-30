#ifndef EXEC_ENV_H
#define EXEC_ENV_H

struct exec_env {
    bool background_enabled;
};

struct exec_env* init_exec_env(void);

#endif