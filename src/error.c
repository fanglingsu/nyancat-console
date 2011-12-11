#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <unistd.h>
#include "error.h"
#include "main.h"

/**
 * @format: Formatstring like in fprintf.
 *
 * Signal a fatal error and quit immediately.
 */
void fatal_error(const char *format, ...)
{
    va_list ap;

    /* clean all ncurses windows */
    cleanup_windows();

    va_start(ap, format);
    fputs("Fatal error: ", stderr);
    vfprintf(stderr, format, ap);
    fputc('\n', stderr);
    va_end(ap);

    exit(EXIT_FAILURE);
}

