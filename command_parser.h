#ifndef COMMAND_PARSER_H   /* Include guard */
#define COMMAND_PARSER_H


#include "commands.h"

/**
 * Parses a command string into a structure describing word expansion.
 * @param buffer used buffer to substitute command string.
 * @param command_str unparsed string command.
 * @return parsed command.
 */
command_t parse_command(char *buffer, const char* command_str);



#endif // COMMAND_PARSER_H