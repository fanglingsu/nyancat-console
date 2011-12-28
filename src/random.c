/**
 * nyancat-console - a clone of the nyancat lost in space flash game.
 *
 * Copyright (C) 2011 Daniel Carl
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
#include <time.h>
#include <assert.h>
#include "random.h"

/**
 * Initializes the random number generator with time based seed.
 */
void random_init(void)
{
    srand(time(0));
}

/**
 * Retrieves a random number in range from given floor till ceil.
 */
int random_range(unsigned floor, unsigned ceil)
{
    assert(ceil > 0);
    return floor + (int)((ceil + 0.5 - floor) * rand() / (RAND_MAX + 1.0));
}

/**
 * Retrieves a random number in range from given floor till ceil in given
 * steps.
 */
int random_range_step(unsigned floor, unsigned ceil, int step)
{
    assert(step > 0);
    return random_range((int)(floor / step), (int)(ceil / step)) * step;
}
