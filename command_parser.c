#include <malloc.h>
#include <memory.h>
#include <ctype.h>
#include "strutil.h"
#include "command_parser.h"
#include "variables.h"

#define BUF_SIZE 1024

void set_mode(command_t *command);

void substitute(char *buffer, char *command_str);

int is_comment(const char *command_str);

command_t parse_command(char *buffer, const char *command_str) {
    command_t command;
    if (is_comment(command_str)) {
        command.type = COMMENT;
        command.argc = 1;
        command.args = (char **) strdup(command_str);
        return command;
    }
    substitute(buffer, command_str);
    char *buf_dup = malloc(BUF_SIZE * sizeof(char));

    strcpy(buf_dup, buffer);
    if (contains(buffer, '=')) {
        char **assign_args = malloc(BUF_SIZE * sizeof(char *));
        size_t argc = split(buffer, assign_args, EQUALS_DELIM);
        command.type = ASSIGNMENT;
        command.argc = argc;
        command.args = assign_args;
        set_mode(&command);
        free(buf_dup);
        return command;
    }
    strcpy(buffer, buf_dup);
    free(buf_dup);

    char **args = malloc(BUF_SIZE * sizeof(char *));
    size_t argc = split(buffer, args, WHITESPACE_DELIM);
    command.argc = argc;
    command.args = args;
    command.type = INSTRUCTION;
    set_mode(&command);
    return command;
}

void set_mode(command_t *command) {
    if (command->argc == 0 || strcmp(command->args[command->argc - 1], "&") != 0)
        command->mode = FOREGROUND;
    else {
        command->args[--command->argc] = '\0';
        command->mode = BACKGROUND;
    }
}

/**
 * Substitutes $ notations with their equivalent variables.
 * @param buffer  buffer to store the resulted string in.
 * @param command_str command.
 */
void substitute(char *buffer, char *command_str) {
    if (command_str == NULL)
        return;
    enum {
        IN_STR, SUB
    } state = IN_STR;
    char *command_str_cpy = (char *) malloc(10 * BUF_SIZE);
    strcpy(command_str_cpy, command_str);

    char *p, *start;
    char *b = buffer;
    for (p = command_str_cpy; *p != '\0'; p++) {
        int c = (unsigned char) *p;
        switch (state) {
            case IN_STR:
                if (c == '~') {
                    *p = 0;
                    char *var = lookup_variable("HOME");
                    strncat(b, var, strlen(var));
                    b += strlen(var);
                } else if (c == '$') {
                    state = SUB;
                    start = p + 1;
                } else {
                    *b = (char) c;
                    b++;
                }
                break;
            case SUB:
                if (!isalnum(c)) {
                    *p = 0;
                    char *var = lookup_variable(start);
                    strncat(b, var, strlen(var));
                    b += strlen(var);
                    *b = c;
                    b++;
                    state = IN_STR;
                }
                break;
        }
    }
    if (state == SUB) {
        char *var = lookup_variable(start);
        if (var != NULL) {
            strcat(b, var);
            b += strlen(var);
        }
    }
    *b = 0;
    free(command_str_cpy);
}

/**
 * Checks if a given command is a comment.
 * @param comment_str command
 * @return 1 if the given command is a comment, 0 otherwise.
 */
int is_comment(const char *comment_str) {
    if (comment_str == NULL)
        return 0;
    return comment_str[0] == '#';
}