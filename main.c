#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <termios.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>
#include <memory.h>
#include "environment.h"
#include "commands.h"
#include "input_util.h"
#include "command_parser.h"
#include "variables.h"
#include "file_processing.h"
#include "logger.h"

#define BUF_SIZE 1024
#define ANSI_COL_GREEN "\x1b[1;32m"
#define ANSI_COLOR_BLUE "\x1b[1;34m"
#define ANSI_COL_RESET "\x1b[0m"
#define INFO "INFO"
#define ERROR "ERROR"


typedef enum {
    false = 0, true = 1
} bool;

void init_shell();

int run_shell(int, char**);

void console_loop();

void file_loop(int, char**);

void exit_shell(int);

extern pid_t pid = -10;
pid_t shell_pid;
pid_t shell_pgid;
struct termios shell_tmodes;
struct sigaction act_child;
struct sigaction act_int;
int shell_terminal;

void handle_child_sig(int p);

void handle_int_sig(int p);

int main(int argc, char *argv[]) {
    init_shell();
    run_shell(argc, argv);
}

void init_shell() {
    shell_pid = getpid();
    shell_terminal = STDIN_FILENO;

    // Prohibit the shell from running as a background process.
    while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
        kill(shell_pid, SIGTTIN);

    // Set up signal handlers.
    act_child.sa_handler = handle_child_sig;
    act_child.sa_flags = SA_RESTART;
    act_int.sa_handler = handle_int_sig;

    if (sigaction(SIGCHLD, &act_child, 0) == -1) {
        perror("ERROR");
        log_s(ERROR, strerror(errno));
    }
    if (sigaction(SIGINT, &act_int, 0) == -1) {
        perror("ERROR");
        log_s(ERROR, strerror(errno));
    }

    // Set process group for shell.
    setpgid(shell_pid, shell_pid);
    shell_pgid = getpgrp();
    if (shell_pgid != shell_pid) {
        char s[BUF_SIZE];
        sprintf(s, "Cannot put Shell in an independent process group: %s", strerror(errno));
        perror(s);
        log_s(ERROR, s);
        exit(EXIT_FAILURE);
    }

    tcsetpgrp(shell_terminal, shell_pgid);
    tcgetattr(shell_terminal, &shell_tmodes);
}

void handle_child_sig(int p) {
    log_s(INFO, "SIGCHLD received");
    // Wait for child process to terminate.
    while (waitpid(-1, NULL, WNOHANG) > 0) {
    }
}

void handle_int_sig(int p) {
    log_s(INFO, "SIGINT received");
    if (kill(pid, SIGTERM) == 0) {
        // Kill process.
        char s[512];
        sprintf(s, "Process with pid %d was killed", pid);
        log_s(INFO, s);
    }
}

int run_shell(int argc, char **argv) {
    if (argc == 1) {
        console_loop();
        return EXIT_SUCCESS;
    } else if (argc == 2) {
        file_loop(argc, argv);
        return EXIT_SUCCESS;
    }
    exit_shell(EXIT_FAILURE);
}

void file_loop(int argc, char **argv) {
    char *path;
    if (argv[1][0] == '/' || argv[1][0] == '~')
        path = argv[1];
    else {
        char cwd[BUF_SIZE];
        path = getcwd(cwd, sizeof(cwd));
        strcat(path, "/");
        strcat(path, argv[1]);
    }
    if (open_commands_batch_file(path)) {
        char s[512];
        sprintf(s, "Couldn't open file %s: %s\n", path, strerror(errno));
        fprintf(stderr, "ERROR: %s\n", s);
        log_s(ERROR, s);
        exit_shell(EXIT_FAILURE);
    }
    setup_environment();

    FILE *file = get_commands_batch_file();
    char *line;

    do {
        int st;
        line = getline_io(file, &st);
        if (line == NULL) {
            exit_shell(EXIT_SUCCESS);
        }
        printf(ANSI_COLOR_BLUE "%s" ANSI_COL_RESET "\n", line);
        char *buffer = calloc(BUF_SIZE, sizeof(char));
        command_t command = parse_command(buffer, line);
        exec(command);
        free_command(&command);
        free(buffer);
        free(line);
    } while (true);
}

void console_loop() {
    setup_environment();
    char cwd[BUF_SIZE];
    char *line;
    int status = 1;
    do {
        printf(ANSI_COL_GREEN "%s@shell:" ANSI_COL_RESET ANSI_COLOR_BLUE "%s" ANSI_COL_RESET "$ ",
               lookup_variable("USER"), getcwd(cwd, sizeof(cwd)));
        int inp_st;
        line = getline_io(stdin, &inp_st);
        if (line == NULL) {
            printf("\n");
            if (inp_st == 1) {
                exit_shell(EXIT_SUCCESS);
            }
        }
        char *buffer = calloc(BUF_SIZE, sizeof(char));
        command_t command = parse_command(buffer, line);
        exec(command);
        write_to_history_file(line);
        free_command(&command);
        free(buffer);
        free(line);
    } while (status);
}

void exit_shell(int i) {
    char s[512];
    sprintf(s, "Shell exited with exit code %d", i);
    log_s(INFO, s);
    close_history_file();
    close_commands_batch_file();
    close_logger();
    exit(i);
}