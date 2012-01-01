/**
 * nyancat-console - a clone of the nyancat lost in space flash game.
 *
 * Copyright (C) 2011-2012 Daniel Carl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */
#include <stdlib.h>
#include "util.h"
#include "main.h"

/**
 * Try to allocate memory, if fails this function does not return NULL.
 * Instead the game is gracefull aborted.
 */
void *xmalloc(size_t size)
{
    void *ret = malloc(size);
    if (NULL == ret) {
        error_exit("Could not allocate %lu bytes of memory", (unsigned long)size);
    }
    return ret;
}
