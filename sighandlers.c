#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "exec_env.h"
#include "launch.h"
#include "sighandlers.h"

#define DEBUG_SIG true

/* function register_signal
is a helper function that takes a signal specifier and a callback that will
execute when the given signal is passed from the kernal to the process.
Handles struct definition and assignment as a convenience for wrapper functions
for specific signals.
*/
void register_signal(int signal, void (*handler)(int))
{
    struct sigaction catch = { 0 };
    catch.sa_handler = handler;
    catch.sa_flags = 0;

    sigfillset(&catch.sa_mask);
    sigaction(signal, &catch, NULL);
}

/* function register_SIGINT
defines a new sigaction struct and registers the provided handler to
catch SIGINT. Alternatively pass SIG_IGN or SIG_DFL to either ignore or
reset to default behavior. All other signals blocked while handler
executes and no other flags are set.
*/
void register_SIGINT(void (*handler)(int))
{
    register_signal(SIGINT, handler);
}

/* function register_SIGCHLD
defines a new sigaction struct and registers the provided handler to
catch SIGCHLD. Alternatively pass SIG_IGN or SIG_DFL to either ignore or
reset to default behavior. All other signals blocked while handler executes and no other
flags are set.
*/
void register_SIGCHLD(void (*handler)(int))
{
    register_signal(SIGCHLD, handler);
}

/* function register_SIGTSTP
defines a new sigaction struct and registers the provided handler to catch SIGTSTP.
Alternatively pass SIG_IGN or SIG_DFL to either ignore or reset to default behavior.
All other signals blocked while handler executes and no other
flags are set.
*/
void register_SIGTSTP(void (*handler)(int))
{
    register_signal(SIGTSTP, handler);
}

/* function catch_and_do_nothing
is a signal handler that can be registered to catch and do nothing with a given signal.
useful for debugging to confirm signal registration is correct. HOWEVER do not
use instead of SIGINT as the signal for terminating processes will be different if bubbled
up from a child process. presents a newline to stdout file. optional debug message to confirm working.
*/
void catch_and_do_nothing(int signal)
{
    if (DEBUG_SIG) {
        write(STDOUT_FILENO, "\nIgnoring Signal ", 17);
    }

    char* signal_desc = strsignal(signal);
    int i = 0;
    while (*(signal_desc + i) != '\0') {
        write(STDOUT_FILENO, (signal_desc + i), 1);
        i++;
    }

    write(STDOUT_FILENO, "\n", 2);
}

/* function cleanup_children
takes a signal specifier and reports out signal status information
upon termination of child processes. additional diagnostic information
is reported if DEBUG_SIG flag is set.
*/
void cleanup_children(int signal)
{

    if (DEBUG_SIG) {

        write(STDOUT_FILENO, "(cleanup_children) ", 19);

        char* signal_desc = strsignal(signal);
        int i = 0;
        while (*(signal_desc + i) != '\0') {
            write(STDOUT_FILENO, (signal_desc + i), 1);
            i++;
        }

        write(STDOUT_FILENO, "\n", 2);
    }

    char* output = malloc(128);
    char* exit_condition = malloc(128);
    int status;
    int pid = waitpid(-1, &status, WNOHANG);

    switch (pid) {
    case -1:
        if (DEBUG_SIG) {
            perror("(cleanup_children) (waitpid)");
            fflush(stdout);
        }

        break;

    case 0:

        if (DEBUG_SIG) {
            write(STDOUT_FILENO, "no child processes have exited since last check\n", 49);
        }

        break;

    // report out background process termination
    default:

        if WIFEXITED (status) {
            sprintf(exit_condition, "exited value %d\n", WEXITSTATUS(status));
        }

        if WIFSIGNALED (status) {
            sprintf(exit_condition, "terminated by signal %d\n", WTERMSIG(status));
        }

        sprintf(output, "background pid %d is done: ", pid);
        write(STDOUT_FILENO, output, 128);
        write(STDOUT_FILENO, exit_condition, 128);
        break;
    }
}

/* function toggle_background
accepts a signal identifier (unused but required by sigaction sa_handler callback definition)
and toggles a global background_enabled bool held in an exec_env struct. this bool is a shared
state for the entire shell and is meant to be read by the launcher routine to determine
behavior of the & background operator.
*/
void toggle_background(int signal)
{
    extern struct exec_env* exec_env;
    exec_env->background_enabled = !(exec_env->background_enabled);

    switch ((int)exec_env->background_enabled) {
    case true:
        write(STDOUT_FILENO, "\nExiting foreground-only mode\n", 30);
        break;

    case false:
        write(STDOUT_FILENO, "\nEntering foreground-only mode (& is now ignored)\n", 50);
        break;

    default:
        break;
    }
}