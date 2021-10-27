/*

input.h - definitions and documentation for input.c
Author: Richie Stuver
Course: CS344 - Operating Systems I
Created: 10-27-21

*/

#define PROMPT ": "
#define DEBUG_INPUT false

/* function display_prompt
prints the prompt string to stdout. return 0 on success or -1 on some failure condition. flushes stdout after each call
in order to make sure buffer is really written to terminal.

returns: integer indicating success (0) or failure (-1)
*/
int display_prompt(void);

/* function get_user_input
retrieves input from stdin and returns a pointer to this input.

returns: pointer to user input. return value will be NULL if user input is empty (e.g. CTRL-D to send EOF).
*/
char* get_user_input(void);

/* function strips newlines
strips all newlines from input string and returns a version with no newlines.
char* string: contains input string presumably with newlines.
returns: pointer to a new string, with newlines removed. returns NULL if line is now empty
after removing a singular newline
*/
char* strip_newlines(char* source);

/* function strip_comments
parses source character array and strips out all content following a #
returns only the content of source array found prior to the first encountered #

source: input character array
returns: pointer to new character array representing the content prior to #
if stripping a single # leaves an empty character array, will return NULL.
*/
char* strip_comments(char* source);