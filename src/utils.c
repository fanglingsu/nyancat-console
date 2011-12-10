#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

/**
 * Reads the content if file given by @filepath into also given @contents.
 */
bool file_get_contents(const char *filepath, char **contents)
{
    FILE *fp;
    char *str;
    char buf[2048];
    size_t bytes, total_bytes, total_allocated;

    assert(filepath != NULL);

    fp = fopen(filepath, "r");
    if (fp == NULL) {
        fprintf(stderr, "Could not open file %s", filepath);
        return false;
    }

    /* allocate memory for the file contents */
    total_bytes = 0;
    total_allocated = 64;
    str = malloc(total_allocated);

    while (!feof(fp)) {
        bytes = fread(buf, 1, 2048, fp);
        while ((total_bytes + bytes + 1) > total_allocated) {
            total_allocated *= 2;
            str = realloc(str, total_allocated);

            if (str == NULL) {
                fprintf(
                    stderr,
                    "Could not allocate %lu bytes to read file \"%s\"",
                    (long)total_allocated,
                    filepath
                );
                goto error;
            }
            if (ferror(fp)) {
                fprintf(stderr, "Error reading file '%s'", filepath);
                goto error;
            }
        }
        memcpy(str + total_bytes, buf, bytes);
        total_bytes += bytes;
    }

    fclose(fp);
    str[total_bytes] = '\0';
    *contents = str;

    return true;

    error:

    free(str);
    fclose(fp);
    return false;
}
