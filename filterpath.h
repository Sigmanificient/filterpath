#ifndef FILTERPATH_H
    #define FILTERPATH_H

    #include <unistd.h>

    #define length_of(s) (sizeof (s) / sizeof ((s)[0]))

    #define sstr_len(s) (length_of(s) - 1)
    #define sstr_unpack(s) (s), (sstr_len(s))

typedef struct {
    char *line;
    ssize_t len;
} input_t;

#endif
