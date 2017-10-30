
#ifndef SHELL_LOGGER_H
#define SHELL_LOGGER_H

/**
 * Set up logger.
 */
void init_logger();

/**
 * Close logger files.
 */
void close_logger();

/**
 * Push a new line to log file.
 * @param tag "ERROR/INFO/DEBUG"
 * @param s line to be put in log file.
 */
void log_s(char *tag, char *s);

#endif //SHELL_LOGGER_H
