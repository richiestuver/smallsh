#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

/* function display_prompt
prints the prompt string to stdout. return 0 on success or -1 on some failure condition. flushes stdout after each call
in order to make sure buffer is really written to terminal.

returns: integer indicating success (0) or failure (-1)
*/
int display_prompt(void)
{
    if (!(printf(PROMPT) >= 0)) {
        printf("(display_prompt) could not display the prompt!");
        return -1;
    };

    if (fflush(stdout) != 0) {
        printf("(display_prompt) error trying to flush stdout!");
        return -1;
    };

    return 0;
}

/* function get_user_input
retrieves input from stdin and returns a pointer to this input.

returns: pointer to user input. return value will be NULL if user input is empty (e.g. CTRL-D to send EOF).
*/
char* get_user_input(void)
{
    char* buffer = NULL;
    size_t n = 0;

    // CTRL-D sends EOF signal and sets error indicator on stdin
    // stdin will stay in this state until it is reset so any future
    // calls to get_user_input will trigger the following failure
    // using clearerr to reset stdin stream to a clean state
    if (getline(&buffer, &n, stdin) == -1) {
        if (DEBUG_INPUT) {
            printf("\n(get_user_input) error getting user input...\n");
            if (feof(stdin) != 0) {
                printf("(get_user_input) EOF condition reached...\n");
            }
        }
        clearerr(stdin);
        return NULL;
    };

    if (DEBUG_INPUT) {
        printf("(get_user_input) received command: %s\n", buffer);
    }

    return buffer;
}

/* function strips newlines
strips all newlines from input string and returns a version with no newlines.
char* string: contains input string presumably with newlines.
returns: pointer to a new string, with newlines removed. returns NULL if line is now empty
after removing a singular newline
*/
char* strip_newlines(char* source)
{

    if (source == NULL) {
        return NULL;
    }

    char* token = NULL;
    char* save_ptr = NULL;
    char* output = NULL;
    char* tokenize = NULL;

    tokenize = calloc(sizeof(char) * (strlen(source) + 1), sizeof(char));
    output = calloc(sizeof(char) * (strlen(source) + 1), sizeof(char));
    strcpy(tokenize, source);

    if ((token = strtok_r(tokenize, "\n", &save_ptr)) != NULL) {
        strcat(output, token);
        while ((token = strtok_r(NULL, "\n", &save_ptr)) != NULL) {
            strcat(output, token);
        }
    }

    if (DEBUG_INPUT) {
        printf("(strip_newlines) source: %s\n", source);
        printf("(strip_newlines) output: %s\n", output);
        printf("(strip_newlines) strlen of output: %lu\n", strlen(output));
    }

    if (strlen(output) == 0) {
        return NULL;
    }

    return output;
}

/* function strip_comments
parses source character array and strips out all content following a #
returns only the content of source array found prior to the first encountered #

source: input character array
returns: pointer to new character array representing the content prior to #
if stripping a single # leaves an empty character array, will return NULL.
*/
char* strip_comments(char* source)
{

    if (source == NULL) {
        return NULL;
    }

    char* output = malloc(sizeof(char) * (strlen(source) + 1));
    strcpy(output, source);
    char* comment_loc = strstr(output, "#");

    if (comment_loc != NULL) {

        if (DEBUG_INPUT) {
            printf("(strip_comments) comment: %s\n", comment_loc);
        }

        *comment_loc = '\0';
    }

    if (DEBUG_INPUT) {
        printf("(strip_comments) source: %s\n", source);
        printf("(strip_comments) output: %s\n", output);
    }

    if (strlen(output) == 0) {
        return NULL;
    }

    return output;
}