/*

Title: Assignment 3: smallsh
Author: Richie Stuver
Course: CS344 - Operating Systems I
Created: 10-20-21

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "parser.h"
#include "smallsh.h"

int main(void)
{
    char* user_input;
    struct command* command;
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

        command = parse(user_input);
        printf("(main) executing %s\n", user_input);
    }
    return 0;
}