/*

Title: Assignment 3: smallsh
Author: Richie Stuver
Course: CS344 - Operating Systems I
Created: 10-20-21

*/

#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
#include "input.h"
#include "launch.h"
#include "parser.h"
#include "smallsh.h"

int main(void)
{
    char* user_input;
    struct command* command;

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

        if (DEBUG) {
            printf("(main) fully parsed command struct is: \n");
            printf("        command (arg0): %s\n", *(command->argv + 0));
            int i = 1;
            while (*(command->argv + i) != NULL) {
                printf("        arg%d: %s\n", i, *(command->argv + i));
                i += 1;
            }

            printf("        background: %s\n", (command->background == 1) ? "true" : "false");
            printf("        stdin: %s\n", (command->f_stdin));
            printf("        stdout: %s\n", (command->f_stdout));
            printf("        stderr: %s\n", (command->f_stderr));
        }

        launch(command);
    }
    return 0;
}