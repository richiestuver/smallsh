#ifndef SIGHANDLERS_H
#define SIGHANDLERS_H

#define DEBUG_SIG true

/* function catch_and_do_nothing
is a signal handler that can be registered to catch and do nothing with a given signal.
useful for debugging to confirm signal registration is correct. HOWEVER do not
use instead of SIGINT as the signal for terminating processes will be different if bubbled
up from a child process. presents a newline to stdout file. optional debug message to confirm working.
*/
void catch_and_do_nothing(int signal);

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

void toggle_background(int signal);

#endif