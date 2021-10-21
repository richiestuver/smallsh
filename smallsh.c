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
retrieves input from stdin, strips out any newline characters, and returns a pointer to this input. 

returns: pointer to cleansed user input. return value will be NULL if user input is empty or newline only. 
*/
char* get_user_input(void)
{
    ssize_t num_chars_read;
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

    return buffer;
}

int main(void)
{
    char* user_input;
    printf("Hello World!\n");

    while (true) {
        display_prompt();

        if ((user_input = get_user_input()) != NULL) {
            printf("%s", user_input);
        };

        free(user_input);
    }

    return 0;
}