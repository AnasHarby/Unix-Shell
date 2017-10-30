#ifndef VARIABLES_H_   /* Include guard */
#define VARIABLES_H_


#include "array.h"

void init_var_arr();

/**
 * Looks up variable in local table, if not found it looks it up in environment variables, return null if not found.
**/
const char *lookup_variable(const char *key);

/**
	Sets a local variable.
**/
void set_local_var(const char *key, const char *value);

/**
 * Sets an environment variable.
 **/
void set_env_var(const char *key, const char *value);

#endif // VARIABLES_H_