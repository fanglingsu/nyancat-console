#include <stdlib.h>
#include "util.h"
#include "main.h"

/**
 * Try to allocate memory, if fails this function does not return NULL.
 * Instead the game is gracefull aborted.
 */
void *
xmalloc(size_t size)
{
    void *ret = malloc(size);
    if (NULL == ret) {
        error_exit("Could not allocate %lu bytes of memory", (unsigned long)size);        
    }
    return ret;
}
