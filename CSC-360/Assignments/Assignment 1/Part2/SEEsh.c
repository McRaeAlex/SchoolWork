/**
 * Program: Interactive shell for CSC 360
 * Author: Alexander McRae
 * Date: Jan 21, 2019
 */

#include "SEEsh.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

extern char **environ;

void run_program(char **args) {
    pid_t pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) != 0) {
            perror("Failed to run program");
        }
    } else if (pid > 0) {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("Error running program");
            return;
        }
    } else if (pid < 0) {
        perror("Error forking");
    }
}

void set_env(char **args) {
    if (args[1] == NULL) {
        for (int i = 0; environ[i] != NULL; i++) {
            printf("%s\n", environ[i]);
        }
    } else {
        char *val = args[2] ? args[2] : "\"\"";
        if (setenv(args[1], val, 1) != 0) {
            perror("Error setting env variable");
            errno = 0;
        }
    }
}

void unset_env(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "unset expects an arg: unset <variable to unset>\n");
        return;
    }
    unsetenv(args[1]);
}

void change_dir(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Expected arg to cd. In form cd <directory>\n");
        char* HOME = getenv("HOME");
        if (HOME) {
            printf("Changing to HOME directory\n");
            if (chdir(HOME) != 0) {
                perror("Failed to change to the HOME directory");
            }
        }
        return;
    }

    if (chdir(args[1]) != 0) {
        perror("Failed to change directory");
    }
}

#define STR_EQL(s1, s2) (strcmp(s1, s2) == 0)
void interpret(char ** /*string arr */ args, bool *should_close) {
    if (args[0] == NULL) {
        return;
    } else if (STR_EQL(args[0], "exit")) {
        *should_close = true;
    } else if (STR_EQL(args[0], "cd")) {
        change_dir(args); // TODO: go to HOME if args in NULL
    } else if (STR_EQL(args[0], "pwd")) {
        char *cwd = getcwd(NULL, 0);
        printf("%s\n", cwd);
        free(cwd);
    } else if (STR_EQL(args[0], "set")) {
        set_env(args);
    } else if (STR_EQL(args[0], "unset")) {
        unset_env(args);
    } else if (STR_EQL(args[0], "help")) {
        printf(
           "cd [dir]: changes the current working directory or if dir is omitted goes to HOME\n \
pwd: prints the full filesname of the current working directory\n \
help: this\n \
exit: Exit the shell\n \
set <var> [value]: Sets an environment variable <var> to [value]. If value is omitted then sets to \"\". If both <var> and [value] are missing it will print the current env out\n \
unset <var>: Destroys the env variable\n"
        );
    } else {
        run_program(args);
    }
}

void init() {
    // Open the file
    char* HOME = getenv("HOME");
    if (!HOME) {
        printf("HOME is not set. No .SEEshrc file will be read.\n");
        return;
    }
    char* path = malloc(strlen(HOME) + strlen("/.SEEshrc") + 1);
    strcpy(path, HOME);
    strcat(path, "/.SEEshrc");

    FILE *config = fopen(path, "r");
    free(path); // Free the path string
    if (errno != 0) {
        // There is no config return
        if (errno == 2) {
            errno = 0; // we have "handled" the error
            return;
        } else {
            perror("Failed to initialize SEEsh. Did not read in rc file");
            errno = 0;
            return;
        }
    }

    bool end_of_file = false;
    while (!end_of_file) {
        char *line = get_line(&end_of_file, config);
        char **args = get_args(line);
        interpret(args, &end_of_file);

        free(line);
        free(args);
    }

    fclose(config);
}

#define whitespace " \t\r\n"
char **get_args(char *line) {
    // Go through the line until the end of the line
    int i = 0;
    int arr_len = 5;
    char ** /* array of strings */ args = malloc(sizeof(char *) * arr_len);

    char *token = strtok(line, whitespace);
    while (token != NULL) {
        args[i++] = token;
        if (i == arr_len) {
            args = realloc(args, sizeof(char *) * (arr_len + 5));
            arr_len = arr_len + 5;
        }
        token = strtok(NULL, whitespace);
    }
    args[i] = NULL; // This is valid as we reallocate before we need to.
                    // This makes the code easier to read.
    return args;
}

// Gets a line from a file descriptor
char *get_line(bool *eof_found, FILE *in) {
    char *line = NULL;
    size_t len;
    if (getline(&line, &len, in) == -1) {
        if (errno == 0) {
            *eof_found = true;
        } else {
            perror("Error reading line");
            free(line);
            exit(-1);
        }
    }
    return line;
}

void loop() {
    while (true) {
        char *cwd = getcwd(NULL, 0);
        printf("%s> ", cwd);
        bool should_close = false;
        char *line = get_line(&should_close, stdin);
        char **args = get_args(line);
        interpret(args, &should_close);

        free(line); // when we dealloc
        free(args);
        free(cwd);
        if (should_close) {
            printf("I hope you enjoyed SEEsh! Good bye!\n");
            return;
        }
    }
}

static void sigint_handler(int signo) {
    printf("To quit the program please type `exit` or press Ctrl + d\n");
}

int main() {
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Failed to set signal handler");
    }

    // start up -> interpret the ~/.SEEshrc if its there
    init();

    loop();

    return 0;
}