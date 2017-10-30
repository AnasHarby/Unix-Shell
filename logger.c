#include <time.h>
#include "logger.h"
#include "file_processing.h"

#define TIME_FORMAT "%Y-%m-%d %H:%M:%S"

void init_logger() {
    open_log_file();
}

void close_logger() {
    close_log_file();
}

void log_s(char *tag, char *s) {
    FILE *log_f = get_log_file();
    if (s == NULL || tag == NULL)
        return;
    if (log_f == NULL)
        return;
    time_t timer;
    char tm_buff[26];
    struct tm *tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(tm_buff, 26, TIME_FORMAT, tm_info);
    fprintf(log_f, "%s -- [%s]: %s\n", tm_buff, tag, s);
}