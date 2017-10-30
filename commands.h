#ifndef COMMANDS_H_   /* Include guard */
#define COMMANDS_H_

#include <glob.h>

/**
 * Defines the mode of the execution for a command.
 */
typedef enum {
    FOREGROUND, BACKGROUND
} mode_e;

/**
 * Defines type of the command.
 */
typedef enum {
    INSTRUCTION, ASSIGNMENT, COMMENT
} type_e;

/*
 * Structure describes a word expansion.
 */
typedef struct {
    size_t argc; // Number of words.
    char **args; // List of words.
    mode_e mode; // mode.
    type_e type;
} command_t;


/**
 * Currently working process pid.
 */
extern int pid;

/**
 * Execute a command.
 * @param command parsed command.
 * @return 0 if succeeded, 0 otherwise.
 */
int exec(const command_t command);

/**
 * Free the memory a command reserves.
 * @param command command to be freed.
 */
void free_command(command_t *command);

#endif // COMMANDS_H_