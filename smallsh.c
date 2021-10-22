/*

Title: Assignment 3: smallsh 
Author: Richie Stuver
Course: CS344 - Operating Systems I
Created: 10-20-21

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEBUG true
#define PROMPT ": "

struct command {
    char* cmd;
    int argc;
    char** argv;
};

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
        if (DEBUG) {
            printf("\n(get_user_input) error getting user input...\n");
            if (feof(stdin) != 0) {
                printf("(get_user_input) EOF condition reached...\n");
            }
        }
        clearerr(stdin);
        return NULL;
    };

    if (DEBUG) {
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

    char* tokenize = malloc(sizeof(char) * (strlen(source) + 1));
    char* output = malloc(sizeof(char) * (strlen(source) + 1));
    strcpy(tokenize, source);

    if ((token = strtok_r(tokenize, "\n", &save_ptr)) != NULL) {
        strcat(output, token);
        while ((token = strtok_r(NULL, "\n", &save_ptr)) != NULL) {
            strcat(output, token);
        }
    }

    if (DEBUG) {
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
        printf("(strip_comments) comment: %s\n", comment_loc);
        *comment_loc = '\0';
    }

    printf("(strip_comments) source: %s\n", source);
    printf("(strip_comments) output: %s\n", output);

    if (strlen(output) == 0) {
        return NULL;
    }

    return output;
}

/* function parse_input
receives an input character array and then delegates to a series of specific parsers to populate a 
command struct representing the command and arguments to be executed by the shell.

input: character array representing target command specified by user. must NOT be NULL
returns: pointer to a populated command struct
*/
struct command* parse_input(char* input)
{
    char* token = NULL;
    char* save_ptr = NULL;

    char* tokenize = malloc(sizeof(char) * (strlen(input) + 1));
    strcpy(tokenize, input);

    printf("(parse_input) parsing: %s\n", tokenize);

    token = strtok_r(tokenize, " ", &save_ptr);

    do {

        printf("(parse_input) found token: %s\n", token);

        switch (token[0]) {

        case '<':
            printf("(parse_input) stdin redirect: <\n");
            break;

        case '>':
            printf("(parse_input) stdout redirect: >\n");
            break;

        case '&':
            printf("(parse_input) background operator: &\n");
            break;

        default:
            break;
        }

    } while ((token = strtok_r(NULL, " ", &save_ptr)) != NULL);

    return;
}

int main(void)
{
    char* user_input;
    printf("Hello World!\n");

    while (true) {
        display_prompt();

        user_input = get_user_input();
        user_input = strip_newlines(user_input);
        user_input = strip_comments(user_input);

        if (user_input == NULL) {
            if (DEBUG) {
                printf("(main) user input is NULL...\n");
            }
            continue;
        };

        parse_input(user_input);
        printf("(main) executing %s\n", user_input);
    }
    return 0;
}