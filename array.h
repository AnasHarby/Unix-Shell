#ifndef SHELL_ARRAY_H
#define SHELL_ARRAY_H

#include <zconf.h>

typedef struct {
    char **arr;
    size_t used;
    size_t size;
} array_t;

void init_arr(array_t *a, size_t init_size);

void insert_arr(array_t *a, char *entry);

void free_arr(array_t *a);

#endif //SHELL_ARRAY_H
