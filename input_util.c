#include <memory.h>
#include <stdio.h>
#include <malloc.h>
#include "input_util.h"

#define BUF_SIZE 1024

char *getline_io(FILE *stream, int *status) {
    char *line;
    line = (char *) malloc(BUF_SIZE);
    if (feof(stdin)) {
        free(line);
        *status = 1;
        return NULL;
    }
    if (!fgets(line, BUF_SIZE, stream)) {
        free(line);
        *status = 2;
        return NULL;
    }
    int len = strlen(line);
    if (len > 0 && line[len - 1] == '\n')
        line[--len] = '\0';
    *status = 0;
    return line;
}