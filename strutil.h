#ifndef SHELL_STRUTIL_H
#define SHELL_STRUTIL_H
#define WHITESPACE_DELIM " \t\r\n\a\f\v"
#define COLON_DELIM ":"
#define EQUALS_DELIM "="

#include <stddef.h>

/**
 * Split a string to multiple tokens using a delimiter.
 * @param str string to be split.
 * @param argv args buffer.
 * @param delim delimiter used to split the string.
 * @return
 */
size_t split(char *str, char **argv, char *delim);

/**
 * Return 1 if a string contains the given char, 0 otherwise.
 */
int contains(char *str, char c);

#endif //SHELL_STRUTIL_H
