#ifndef SEEsh_H
#define SEEsh_H

#include <stdbool.h>
#include <stdio.h>

void set_env(char** args);
void unset_env(char** args);

void change_dir(char** args);

void interpret(char** /*string arr */ args, bool* should_close);

// Runs when the program opens
void init();

#define whitespace " \t\r\n"
// Parses line into tokens over whitespace
char** get_args(char* line);

// Gets a line from a file descriptor
char* get_line(bool* eof_found, FILE* in);

// Main loop
void loop();

#endif
