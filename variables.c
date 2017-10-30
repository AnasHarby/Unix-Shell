#include <stdlib.h>
#include <memory.h>
#include "variables.h"

array_t keys, vals;

void init_var_arr() {
    size_t size = 10;
    init_arr(&keys, size);
    init_arr(&vals, size);
}

const char *lookup_variable(const char *key) {
    for (int i = 0; i < keys.used; i++)
        if (strcmp(keys.arr[i], key) == 0)
            return vals.arr[i];
    return getenv(key);
}

void set_local_var(const char *key, const char *value) {
    for (int i = 0; i < (int) keys.used; i++)
        if (strcmp(keys.arr[i], key) == 0) {
            vals.arr[i] = strdup(value);
            return;
        }
    insert_arr(&keys.arr, strdup(key));
    insert_arr(&vals.arr, strdup(value));
}

void set_env_var(const char *key, const char *value) {
    setenv(key, value, 1);
}