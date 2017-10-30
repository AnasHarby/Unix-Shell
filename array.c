#include <malloc.h>
#include "array.h"

void init_arr(array_t *a, size_t init_size) {
    a->arr = (char *) malloc(init_size * sizeof(char *));
    a->used = 0;
    a->size = init_size;
}

void insert_arr(array_t *a, char *entry) {
    if (a->used == a->size) {
        a->size *= 2;
        a->arr = (char *) realloc(a->arr, a->size * sizeof(char *));
    }
    a->arr[a->used++] = entry;
}

void free_arr(array_t *a) {
    free(a->arr);
    a->arr = NULL;
    a->used = a->size = 0;
}