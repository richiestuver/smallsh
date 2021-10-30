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

void register_signal(int signal);

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

void cleanup_children(int signal);
#endif