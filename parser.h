/*
parser.h - definitions and documentation for parser.c
Author: Richie Stuver
Course: CS344 - Operating Systems I
Created: 10-27-21

*/

#define DEBUG_PARSER false
#define DEV_NULL "/dev/null"

/* function init_empty_command
initializes a new command struct by allocating memory for the struct itself as
well as the argv array. other non-pointer members are initialized.

returns: pointer to an empty command struct with memory allocated.
 */
struct command* init_empty_command();

/* function parse_command
parses an input character array and saves the first space-delimited token
to the given command struct as the first argument to argv.

source: input character array from which to retrieve command
save_ptr: pointer to pointer to modifed source after tokenization
command: pointer to command struct in which to save in argv
returns: save_ptr (modified) - pointer to pointer to remaining content of input array (to be parsed)
*/
char** parse_command(char* source, char** save_ptr, struct command* command);

/* function check_from_start
checks whether the starting n chars of source string contains any full (string) element of array

source: poiunter to string to be checked
targets: array of strings against which to be checked. last element MUST be NULL
for termination purposes
returns: pointer to string containing keyword found or NULL if none found
*/

char* check_from_start(char* source, char** keywords);

/* function check_reserved_words
determines if a token contains a reserved symbols or words at its start and returns the
reserved word or symbol found. order of preference is first encountered.

token: pointer to character array representing an extracted token in parser
returns: character array containing word or symbol
*/
char* check_reserved_words(char* token);

/* function reattach_token
"undo" for a strtok call to reattach a token that was carved out of a string.
does this by replacing the null terminator with the original delimiter char
and resetting the address held by save_ptr to the address of the token that was
split off.

save_ptr: pointer to pointer holding current location in string being tokenized
token: pointer to start of token that was just carved out
delimiter: char representing the delimiter that was replaced by '\0'
*/
char** reattach_token(char** save_ptr, char* token, char delim);

/* function parse_args
receives the address of a pointer to the current location in string being parsed
returns a new pointer to updated location after all args have been extracted
and stored in the command struct. parsing exits if string is exhausted or
a reserved keyword is encountered.

save_ptr: pointer to pointer to string under tokenization
command: pointer to command struct in which to save in argv
returns: save_ptr address
*/
char** parse_args(char** save_ptr, struct command* command);

/* function parse_redirects
receives the address of a pointer to current location in string being parsed
returns a new pointer to updated location after all redirects have been parsed
and stored in command struct. parsing exits if a reserved keyword that is not a
redirect is encountered.

save_ptr: pointer to pointer to stream under tokenization
command: pointer to command struct in which to save in argv
returns: save_ptr address
*/
char** parse_redirects(char** save_ptr, struct command* command);

/* function parse_variable_expansion
receives the address of a pointer to current location in string being parsed
returns a new pointer to updated location after all variables $$ have been parsed
and replaced with process ID of this shell. process ID is reattached to input stream
to be reinterpreted as an argument. parsing exits if a reserved keyword that is not a
a variable is encountered.

save_ptr: pointer to pointer to stream under tokenization
returns: save_ptr address
*/

char** parse_variable_expansion(char** save_ptr);

/* function parse_background_operator
receives the address of a pointer to current location in string being parsed
returns a new pointer to updated location after background operator & has been parsed.
parsing exits and token & is treated as text if not the last token in the stream.

save_ptr: pointer to pointer to stream under tokenization
returns: save_ptr address
*/
char** parse_background_operator(char** save_ptr, struct command* command);

/* function parse
receives an input character array and then delegates to a series of specific parsers to populate a
command struct representing the command and arguments to be executed by the shell.

input: character array representing target command specified by user. must NOT be NULL
returns: pointer to a populated command struct
*/
struct command* parse(char* input);