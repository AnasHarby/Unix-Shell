#include <stdio.h>
#include <malloc.h>
#include "environment.h"
#include "commands.h"
#include "command_parser.h"
#include "variables.h"
#include "file_processing.h"
#include "logger.h"

#define BUF_SIZE 512

void reset_dir();

void setup_environment(void) {
    init_logger();
    log_s("INFO", "Shell started");
    open_history_file();
    init_var_arr();
    reset_dir();
}

void reset_dir() {
    char line[] = "cd ~";
    char *buffer = calloc(BUF_SIZE, sizeof(char));
    command_t command = parse_command(buffer, line);
    exec(command);
    free_command(&command);
    free(buffer);
}
