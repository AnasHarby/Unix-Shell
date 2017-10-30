#include <zconf.h>
#include <memory.h>
#include <malloc.h>
#include "file_processing.h"

FILE *history_file;
FILE *log_file;
FILE *batch_command_file;

void open_history_file() {
    char cwd[512];
    char *path = getcwd(cwd, sizeof(cwd));
    strcat(path, "/history.txt");

    fopen(path, "w");
    close_history_file();
    history_file = fopen(path, "a+");
}

FILE *get_history_file() {
    return history_file;
}

void close_history_file() {
    if (history_file == NULL)
        return;
    if (fclose(history_file) != 0)
        fprintf(stderr, "ERROR: Couldn't close file \"history.txt\"");
}

void write_to_history_file(char *s) {
    if (s == NULL)
        return;
    fprintf(history_file, "%s\n", s);
}

/*
	log file section
*/
void open_log_file() {
    char cwd[512];
    char *path = getcwd(cwd, sizeof(cwd));
    strcat(path, "/log.txt");

    fopen(path, "w");
    close_log_file();
    log_file = fopen(path, "a");
}

FILE *get_log_file() {
    return log_file;
}

void write_to_log(char *s) {
    if (s == NULL)
        return;
    fprintf(log_file, "%s\n", s);
}

void close_log_file() {
    if (log_file == NULL)
        return;
    int status = fclose(log_file);
    if (status != 0)
        fprintf(stderr, "ERROR: Couldn't close file \"log.txt\"");
}


/* 
	CommandsBatch file section
*/
int open_commands_batch_file(char *path) {
    batch_command_file = fopen(path, "r");
    if (batch_command_file == NULL)
        return 1;
    return 0;
}

FILE *get_commands_batch_file() {
    return batch_command_file;
}

void close_commands_batch_file() {
    if (batch_command_file == NULL)
        return;
    int status = fclose(batch_command_file);
    if (status != 0)
        fprintf(stderr, "ERROR: Couldn't close file \"batch.txt\"");
}