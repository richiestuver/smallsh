#define DEBUG_LAUNCH true

/* function launch
accepts a fully filled out command struct and executes the command with provided arguments
and values for stdin, stdout, and stderr. If background is true, command will be run in the background.

command: must be a fully filled command struct.
*/
void launch(struct command* command);