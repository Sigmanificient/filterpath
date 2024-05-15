#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <linux/limits.h>

#include "filterpath.h"

static
const char PATH_BASE_CHARSET[] = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    ".-_/"
};

static
bool shell_readline(input_t *buff)
{
    static size_t zero = 0;

    buff->len = getline(&buff->line, &zero, stdin);
    if (buff->len < 0)
        return false;
    buff->len--;
    buff->line[buff->len] = '\0';
    return true;
}

/**
 * Tries to cut a simple path composedo only by basic characters at it
 * might be the more common type of path to appear.
 * Only match a path that would start with either `.` or `/`
 **/
static
char *cut_simple_path(char *path, input_t *buff)
{
    char *start = buff->line;
    size_t len;

    while (*start != '\0' && strchr("./", *start) == NULL)
        start++;
    if (*start == '\0')
        return NULL;
    len = strspn(start, PATH_BASE_CHARSET);
    memcpy(path, start, len); 
    path[len] = '\0';
    return path;
}

int main(void)
{
    input_t buff;
    char path[PATH_MAX];

    while (shell_readline(&buff)) {
        printf("(%s)\n", buff.line);
        if (cut_simple_path(path, &buff) == NULL)
            continue;
        printf("%s\n", path);
    }
    return EXIT_SUCCESS;
}
