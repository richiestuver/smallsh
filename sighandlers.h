#ifndef SIGHANDLERS_H
#define SIGHANDLERS_H

#include <signal.h>
#define DEBUG_SIG false

/* function catch_and_do_nothing
is a signal handler that can be registered to catch and do nothing with a given signal.
useful for debugging to confirm signal registration is correct. HOWEVER do not
use instead of SIGINT as the signal for terminating processes will be different if bubbled
up from a child process. presents a newline to stdout file. optional debug message to confirm working.
*/
void catch_and_do_nothing(int signal);

/* function block
registers a given signal as the single member of a block set
and calls sigprocmask for the purposes of delaying delivery of the given signal.
this is a convenience wrapper for handling the signal blocking setup.

returns: the sigset used for the blocking mask. use the return value to later unblock.
*/
sigset_t* block(int signal);

/* function unblock
takes a signal set as a parameter and attempts to unblock these signals.
per sigprocmask description, there is no issue attempting to unblock signals
which were already unblocked. passes through parameter unchanged as return value.
*/
sigset_t* unblock(sigset_t* block_set);

/* function register_signal
is a helper function that takes a signal specifier and a callback that will
execute when the given signal is passed from the kernal to the process.
Handles struct definition and assignment as a convenience for wrapper functions
for specific signals.
*/
void register_signal(int signal, void (*handler)(int));

/* function register_SIGINT
defines a new sigaction struct and registers the provided handler to
catch SIGINT. All other signals blocked while handler executes and no other
flags are set.
*/
void register_SIGINT(void (*handler)(int));

/* function register_SIGCHLD
defines a new sigaction struct and registers the provided handler to
catch SIGCHLD. All other signals blocked while handler executes and no other
flags are set.
*/
void register_SIGCHLD(void (*handler)(int));

/* function register_SIGTSTP
defines a new sigaction struct and registers the provided handler to catch SIGTSTP.
Alternatively pass SIG_IGN or SIG_DFL to either ignore or reset to default behavior.
All other signals blocked while handler executes and no other
flags are set.
*/
void register_SIGTSTP(void (*handler)(int));

/* function cleanup_children
takes a signal specifier and reports out signal status information
upon termination of child processes. additional diagnostic information
is reported if DEBUG_SIG flag is set.
*/
void cleanup_children(int signal);

/* function toggle_background
accepts a signal identifier (unused but required by sigaction sa_handler callback definition)
and toggles a global background_enabled bool held in an exec_env struct. this bool is a shared
state for the entire shell and is meant to be read by the launcher routine to determine
behavior of the & background operator.
*/
void toggle_background(int signal);

#endif