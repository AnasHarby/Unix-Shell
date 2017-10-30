#include <memory.h>
#include <ctype.h>

#define BUF_SIZE 1024

int match(char c, char *s) {
    for (int i = 0; i < strlen(s); i++)
        if (c == s[i])
            return 1;
    return 0;
}

size_t split(char *str, char **argv, char *delim) {
    char *p, *start;
    int c;
    enum states {
        DULL, IN_WORD, IN_STR, SUB
    } state = DULL;
    size_t argc = 0;

    for (p = str; *p != '\0'; p++) {
        c = (unsigned char) *p;

        switch (state) {
            case DULL:
                if (isspace(c) || match((char) c, delim))
                    continue;
                else if (c == '"') {
                    state = IN_STR;
                    start = p + 1;
                } else {
                    state = IN_WORD;
                    start = p;
                }
                continue;
            case IN_STR:
                if (c == '"') {
                    *p = 0;
                    argv[argc++] = start;
                    state = DULL;
                }
                continue;
            case IN_WORD:
                if (isspace(c) || match((char) c, delim)) {
                    *p = '\0';
                    char *x = start;
                    argv[argc++] = start;
                    state = DULL;
                }
                continue;
        }
    }
    if (state != DULL)
        argv[argc++] = start;
    argv[argc] = '\0';
    return argc;
}

int contains(char *str, char c) {
    char *p = str;
    while (*p != NULL) {
        if (*p == c)
            return 1;
        p++;
    }
    return 0;
}