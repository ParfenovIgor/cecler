#include "../include/error.h"
#include <stdio.h>
#include <stdlib.h>

void lexerError(const char *str, int line_begin, int position_begin, int line_end, int position_end, const char *filename) {
    fprintf(stderr, "Lexer Error: %s\n%s: %d:%d-%d:%d\n",
        str,
        filename,
        line_begin + 1,
        position_begin + 1,
        line_end + 1,
        position_end + 1);
    exit(1);
}
