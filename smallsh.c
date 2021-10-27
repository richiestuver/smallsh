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
#include <unistd.h>

#define DEBUG true
#define PROMPT ": "

char* RESERVED_WORDS[] = {
    "<",
    ">",
    "&",
    "$$",
    NULL
};

char* REDIRECTS[] = {
    "<",
    ">",
    NULL
};

struct command {
    char* cmd;
    int argc;
    char** argv;
    char* f_stdin;
    char* f_stdout;
    char* f_stderr;
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

/* function init_empty_command
initializes a new command struct by allocating memory for the struct itself as
well as the argv array. other non-pointer members are initialized.

returns: pointer to an empty command struct with memory allocated.
 */
struct command* init_empty_command()
{

    struct command* command;
    command = malloc(sizeof(struct command));
    if (command == NULL) {
        printf("(init_empty_command) error allocating memory for command...");
        return NULL;
    }

    command->argv = calloc(513, sizeof(char*)); // save +1 space for a NULL termination of array
    if (command->argv == NULL) {
        printf("(init_empty_command) error allocating memory for command->argv...");
        return NULL;
    }

    command->argc = 0;

    command->f_stdin = NULL;
    command->f_stdout = NULL;
    command->f_stderr = NULL;

    return command;
}

/* function parse_command
parses an input character array and saves the first space-delimited token
to the given command struct as the first argument to argv.

source: input character array from which to retrieve command
save_ptr: pointer to pointer to modifed source after tokenization
command: pointer to command struct in which to save in argv
returns: save_ptr (modified) - pointer to pointer to remaining content of input array (to be parsed)
*/

char** parse_command(char* source, char** save_ptr, struct command* command)
{
    char* token = NULL;

    token = strtok_r(source, " ", save_ptr);

    // parse commmand
    if (DEBUG) {
        printf("(parse_command) found token: %s\n", token);
    }

    *(command->argv + 0) = token;
    command->argc += 1;

    if (DEBUG) {
        printf("(parse_command) command is: %s\n", *(command->argv + 0));
    }

    return save_ptr;
}

/* function check_from_start
checks whether the starting n chars of source string contains any full (string) element of array

source: poiunter to string to be checked
targets: array of strings against which to be checked. last element MUST be NULL
for termination purposes
returns: pointer to string containing keyword found or NULL if none found
*/

char* check_from_start(char* source, char** keywords)
{
    int i = 0;
    while (*(keywords + i) != NULL) {

        if ((strlen(source)) < strlen(*(keywords + i))) {
            break;
        }

        int cmp_size = strlen(*(keywords + i));

        if (strncmp(source, *(keywords + i), cmp_size) == 0) {

            if (DEBUG) {
                printf("(check_from_start) found: %s\n", *(keywords + i));
            }

            return *(keywords + i);
        }
        i++;
    }

    return NULL;
}

/* function check_reserved_words
determines if a token contains a reserved symbols or words at its start and returns the
reserved word or symbol found. order of preference is first encountered.

token: pointer to character array representing an extracted token in parser
returns: character array containing word or symbol
*/
char* check_reserved_words(char* token)
{
    return check_from_start(token, RESERVED_WORDS);
}

/* function check_redirects
determines if a token contains a redirect symbol at its start and returns the
symbol found. order of preference is first encountered.

token: pointer to character array representing an extracted token in parser
returns: character array containing word or symbol
*/
char* check_redirects(char* token)
{
    return check_from_start(token, REDIRECTS);
}
/* function reattach_token
"undo" for a strtok call to reattach a token that was carved out of a string.
does this by replacing the null terminator with the original delimiter char
and resetting the address held by save_ptr to the address of the token that was
split off.

save_ptr: pointer to pointer holding current location in string being tokenized
token: pointer to start of token that was just carved out
delimiter: char representing the delimiter that was replaced by '\0'
*/
char** reattach_token(char** save_ptr, char* token, char delim)
{
    if (DEBUG) {
        printf("(reattach_token) token: %s\n", token);
        printf("(reattach_token) previous *save_ptr: %s\n", *save_ptr);
        printf("(reattach_token) *save_ptr == empty is %d\n", (strcmp(*save_ptr, "\0") == 0));
    }

    // if the char at save_ptr is "" (empty), we will accidentally overwrite
    // the last char of token with an extraneous delimiter. don't do that.
    if ((strcmp(*save_ptr, "\0") == 0)) {
        *save_ptr = token;
    } else {
        char* loc = *(save_ptr) - sizeof(char) * 1;
        *loc = delim;
        *save_ptr = token;
    }

    if (DEBUG) {
        printf("(reattach_token) new *save_ptr: %s\n", *save_ptr);
        printf("(reattach_token) token: %s\n", token);
        printf("(reattach_token) *save_ptr == empty is %d\n", (strcmp(*save_ptr, "\0") == 0));
    }

    return save_ptr;
}

/* function parse_args
receives the address of a pointer to the current location in string being parsed
returns a new pointer to updated location after all args have been extracted
and stored in the command struct. parsing exits if string is exhausted or
a reserved keyword is encountered.

save_ptr: pointer to pointer to string under tokenization
command: pointer to command struct in which to save in argv
returns: save_ptr address
*/
char** parse_args(char** save_ptr, struct command* command)
{
    char* token = NULL;

    while ((token = strtok_r(NULL, " ", save_ptr)) != NULL) {
        printf("(parse_args) found token: %s\n", token);
        // if (check_from_start(token, RESERVED_WORDS) != NULL) {
        if (check_reserved_words(token) != NULL) {
            save_ptr = reattach_token(save_ptr, token, ' ');
            break;
        };

        if (command->argc >= 512) {
            printf("(parse_args) size limit reached. stopping parse args to prevent buffer overflow...");
            break;
        }

        *(command->argv + command->argc) = token;
        *(command->argv + command->argc + 1) = NULL; // delimits argument list
        command->argc += 1;
    }

    return save_ptr;
}

/* function parse_redirects
receives the address of a pointer to current location in string being parsed
returns a new pointer to updated location after all redirects have been parsed
and stored in command struct. parsing exits if a reserved keyword that is not a
redirect is encountered.

save_ptr: pointer to pointer to string under tokenization
command: pointer to command struct in which to save in argv
returns: save_ptr address
*/
char** parse_redirects(char** save_ptr, struct command* command)
{

    char* token = NULL;

    while ((token = strtok_r(NULL, " ", save_ptr)) != NULL) {
        printf("(parse_redirects) token: %s\n", token);
        printf("(parse_redirects) *save_ptr: %s\n", *save_ptr);

        char* keyword;
        if ((keyword = check_redirects(token)) == NULL) {
            save_ptr = reattach_token(save_ptr, token, ' ');
            break;
        }

        // check if token is a standalone redirect symbol or attached to the value.
        // if attached, need to strip it and store only the value. else value is next token.

        char* value = NULL;
        if (strlen(token) == strlen(keyword)) {
            value = strtok_r(NULL, " ", save_ptr);

        } else {
            value = token + sizeof(char) * 1;
        }

        if (value == NULL || check_reserved_words(value) != NULL) {
            printf("(parse_redirects) syntax error: expected valid filename, not %s\n", value);
            break;
        }

        // add redirect file to command

        if (strcmp(keyword, "<") == 0) {
            if (command->f_stdin == NULL) {
                command->f_stdin = malloc(strlen(value) + 1);
                strcpy(command->f_stdin, value);
            } else {
                printf("(parse_redirects) only one file supported for stdin at this time. dropping %s\n", value);
            }

        } else if (strcmp(keyword, ">") == 0) {
            if (command->f_stdout == NULL) {
                command->f_stdout = malloc(strlen(value) + 1);
                strcpy(command->f_stdout, value);
            } else {
                printf("(parse_redirects) only one file supported for stdout at this time. dropping %s\n", value);
            }
        } else {
            printf("(parse_redirects) fatal error: unsupported redirect: %s\n", keyword);
            exit(1);
        }

        printf("(parse_redirects) value for redirect %s: %s\n", keyword, value);
        printf("(parse_redirects) *save_ptr: %s\n", *save_ptr);
    }

    return save_ptr;
}

/* function parse_variable_expansion
receives the address of a pointer to current location in string being parsed
returns a new pointer to updated location after all variables $$ have been parsed
and replaced with process ID of this shell. process ID is reattached to input stream
to be reinterpreted as an argument. parsing exits if a reserved keyword that is not a
a variable is encountered.

save_ptr: pointer to pointer to string under tokenization
returns: save_ptr address
*/

char** parse_variable_expansion(char** save_ptr)
{

    char* new_str;
    char* token = NULL;
    if ((token = strtok_r(NULL, " ", save_ptr)) != NULL) {
        if (strcmp(token, "$$") != 0) {

            if (DEBUG) {
                printf("(parse_variable_expansion) no $$ found!\n");
            }

            reattach_token(save_ptr, token, ' ');
        } else {

            if (DEBUG) {
                printf("(parse_variable_expansion) found $$\n");
            }

            // assign PID to $$
            char* pid = malloc(sizeof(char) * 12); // allocate 11 chars for digits of PID
            snprintf(pid, 12, "%d", getpid());
            new_str = malloc(strlen(*save_ptr) + strlen(pid) + 2);
            strcpy(new_str, pid);
            strcat(new_str, " ");
            strcat(new_str, *save_ptr);

            if (DEBUG) {
                printf("(parse_variable_expansion) new parse string: %s\n", new_str);
            }

            save_ptr = &new_str;
        }
    }
    return save_ptr;
}

/* function parse
receives an input character array and then delegates to a series of specific parsers to populate a
command struct representing the command and arguments to be executed by the shell.

input: character array representing target command specified by user. must NOT be NULL
returns: pointer to a populated command struct
*/
struct command* parse(char* input)
{

    struct command* command = init_empty_command();

    if (command == NULL) {
        printf("(parse) fatal error - could not initialize command structure. exiting...\n");
        exit(1);
    };

    // char* token = NULL;
    char* save_ptr = NULL;

    char* tokenize = malloc(sizeof(char) * (strlen(input) + 1));
    strcpy(tokenize, input);
    printf("(parse) remaining to be parsed:%s\n", save_ptr);

    // parse command -- WILL call strtok_r. this should be for the FIRST time
    save_ptr = *parse_command(tokenize, &save_ptr, command);

    // TODO: protect against infinite loop, make sure all reserved words are handled.
    do {
        // parse args
        save_ptr = *parse_args(&save_ptr, command);

        // parse redirects
        save_ptr = *parse_redirects(&save_ptr, command);

        save_ptr = *parse_variable_expansion(&save_ptr);

        // current loc in string

        if (DEBUG) {
            printf("(parse) remaining to be parsed:%s\n", save_ptr);
            printf("(parse) *save_ptr is empty is %d\n", (strcmp(save_ptr, "\0") == 0));
        }

    } while (strcmp(save_ptr, "\0") != 0);

    // print command for debug
    if (DEBUG) {
        printf("(parse) fully parsed command struct is: \n");
        printf("        command (arg0): %s\n", *(command->argv + 0));
        int i = 1;
        while (*(command->argv + i) != NULL) {
            printf("        arg%d: %s\n", i, *(command->argv + i));
            i += 1;
        }

        printf("        stdin: %s\n", (command->f_stdin));
        printf("        stdout: %s\n", (command->f_stdout));
        printf("        stderr: %s\n", (command->f_stderr));
    }

    return command;
}

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