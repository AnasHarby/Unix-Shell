#ifndef SHELL_CONSOLE_PROCESSING_H
#define SHELL_CONSOLE_PROCESSING_H

/**
 * Get line from input stream.
 * @param stream input stream (file stream or stdin).
 * @param status equals 0 if succeeded, 1 if the line read is issued by a EOF, 2 otherwise.
 * @return line.
 */
char *getline_io(FILE *stream, int *status);

#endif //SHELL_CONSOLE_PROCESSING_H
