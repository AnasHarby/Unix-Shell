#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <zconf.h>
#include <wait.h>
#include <errno.h>
#include <stdbool.h>
#include "commands.h"
#include "strutil.h"
#include "variables.h"
#include "file_processing.h"
#include "input_util.h"
#include "logger.h"

int exec_in(const char **args, const mode_e mode);

int cd(const char **args);

int echo(const char **args);

int export(const char **args);

int history(const char **args);

int exit_sh(const char **args);

int help(const char **args);

static const char *builtin_commands_str[] = {"cd", "echo", "exit", "export", "history", "help"};

int (*builtin_func[6])(char **args) = {&cd, &echo, &exit_sh, &export, &history, &help};

int exec(const command_t command) {
    if ((unsigned long) command.argc == 0)
        return EXIT_SUCCESS; // Empty command.

    char *command_init = command.args[0];
    static int num_builtins = sizeof(builtin_commands_str) / sizeof(char *);
    switch (command.type) {
        case ASSIGNMENT:
            if (strcmp(command_init, "export") == 0)
                export(command.args);
            else
                set_local_var(command.args[0], command.args[1]);
            break;
        case INSTRUCTION:
            // Try to find the command in builtin functions.
            for (int i = 0; i < num_builtins; i++)
                if (strcmp(command_init, builtin_commands_str[i]) == 0)
                    return (*builtin_func[i])(command.args);
            // Execute internally.
            return exec_in(command.args, command.mode);
        default:
            break;
    }
    return EXIT_SUCCESS;
}

/**
 * Execute internal functions by forking a separate process.
 * @param args args to be fed to the instruction.
 * @param mode mode of execution (Foreground/Background).
 * @return 0 if succeeded, 1 otherwise.
 */
int exec_in(const char **args, const mode_e mode) {
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process, execute
        if (execv(args[0], args) == EXIT_SUCCESS) {
            exit(EXIT_SUCCESS);
        }
        char **paths = malloc(512 * sizeof(char *));
        split(lookup_variable("PATH"), paths, COLON_DELIM);
        char **p = paths;
        while (*p != NULL) {
            char *x = malloc(strlen(*p) + strlen(args[0]) + 2);
            strcpy(x, *p);
            strcat(x, "/");
            strcat(x, args[0]);
            if (execv(x, args) == EXIT_SUCCESS) {
                free(paths);
                free(x);
                exit(EXIT_SUCCESS);
            }
            free(x);
            p++;
        }
        free(paths);
        char s[512];
        sprintf(s, "Invalid command [%s]: %s", args[0], strerror(errno));
        fprintf(stderr, "ERROR : %s: %s\n", args[0], strerror(errno));
        log_s("ERROR", s);
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process, wait
        char s[512];
        switch (mode) {
            case FOREGROUND:
                if (waitpid(pid, &status, 0) != pid)
                    status = -1;
                sprintf(s, "Command [%s] executed in foreground mode", args[0]);
                log_s("INFO", s);
                break;
            default:
                sprintf(s, "Command [%s] executed in background mode", args[0]);
                log_s("INFO", s);
                break;
        }
    } else
        status = -1;
    return status;
}

int cd(const char **args) {
    if (args == NULL || (strcmp(args[0], "cd") != 0))
        return EXIT_FAILURE;
    else if (args[1] == NULL) {
        char *home_args[] = {"cd", "/"}; // To home
        cd(home_args);
    } else if (chdir(args[1]) != 0) {
        perror("ERROR: cd: ");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int echo(const char **args) {
    if (args == NULL || (strcmp(args[0], "echo") != 0))
        return EXIT_FAILURE;
    char **p = args + 1;
    while (*p != NULL) {
        printf("%s ", *p);
        p++;
    }
    printf("\n");
    return EXIT_SUCCESS;
}

int export(const char **args) {
    if (args == NULL || (strcmp(args[0], "export") != 0))
        return EXIT_FAILURE;
    char **p = args + 1;
    while (*p != NULL && *(p + 1) != NULL) {
        set_env_var(*p, *(p + 1));
        p += 2;
    }
}

int exit_sh(const char **args) {
    log_s("INFO", "Shell exited with exit code 0");
    close_commands_batch_file();
    close_history_file();
    close_logger();
    exit(EXIT_SUCCESS);
};

int history(const char **args) {
    FILE *history_f = get_history_file();
    fflush(history_f);
    rewind(history_f);
    char *line;
    int st;
    while (true) {
        line = getline_io(history_f, &st);
        if (line == NULL)
            break;
        fprintf(stdout, "%s\n", line);
        free(line);
    }
}

int help(const char **args) {

}

void free_command(command_t *command) {
    free(command->args);
    command->argc = 0;
}