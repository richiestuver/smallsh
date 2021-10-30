/*

smallsh.h - definitions and documentation for smallsh.c
Author: Richie Stuver
Course: CS344 - Operating Systems I
Created: 10-27-21

*/

#ifndef SMALLSH_H
#define SMALLSH_H

#define DEBUG false

/* function main
is the entrypoint and driver for the smallsh shell. sets initial program state for data structures
that will be used across execution. registers expected signal behaviors for the shell
when prompt is active. launches a "REPL" to collect user input, parse, and execute commands.
*/
int main(void);

#endif
