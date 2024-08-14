#include <getopt.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <linux/limits.h>

#include "filterpath.h"

#define VERSION "1.0.1"

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

static
bool is_valid_path(char const *path)
{
    static char abs[PATH_MAX];

    if (realpath(path, abs) == NULL)
        return false;
    return access(abs, F_OK) == 0;
}

/**
 * Tries to cut a simple path composedo only by basic characters at it
 * might be the more common type of path to appear.
 * Only match a path that would start with either `.` or `/`
 **/
static
bool cut_simple_path(char *path, input_t *buff)
{
    char *start = buff->line;
    size_t len;

    while (*start != '\0' && strchr("./", *start) == NULL)
        start++;
    if (*start == '\0')
        return false;
    len = strspn(start, PATH_BASE_CHARSET);
    memcpy(path, start, len); 
    path[len] = '\0';
    return is_valid_path(path);
}

static
bool find_path_rtrim(char *path, size_t len)
{
    while (path[--len] != '/') {
        path[len] = '\0';
        if (is_valid_path(path))
            break;
    }
    return path;
}

static
bool find_path(char *path, char *line)
{
    size_t len = *line == '/';

    if (strchr(line, '/') == NULL)
        return false;
    for (; line[len] != '/'; len++);
    memcpy(path, line, len);
    path[len] = '\0';
    if (!is_valid_path(path))
        return find_path(path, ++line);
    for (size_t add; line[len] != '\0';) {
        add = 1 + strcspn(&line[len + 1], "/");
        memcpy(&path[len], &line[len], add);
        path[len + add] = '\0';
        if (!is_valid_path(path))
            return find_path_rtrim(path, len + add);
        len += add;
    }
    return true;
}

static
struct option LONG_OPTIONS[] = {
    { "version", no_argument, NULL, 'v' },
    { 0 }
};

int main(int argc, char **argv)
{
    input_t buff = { .len = 0, .line = NULL };
    char path[PATH_MAX];
    int c;

    while ((c = getopt_long(argc, argv, "v", LONG_OPTIONS, NULL)) > 0) {
        switch (c) {
            case 'v':
                return write(STDOUT_FILENO,
                    sstr_unpack("filterpath, version " VERSION "\n")),
                       EXIT_SUCCESS;
            case '?':
                return EXIT_FAILURE;
            default:
                break;
        }
    }
    while (shell_readline(&buff))
        if (cut_simple_path(path, &buff) || find_path(path, buff.line))
            printf("%s\n", path);
    return EXIT_SUCCESS;
}
