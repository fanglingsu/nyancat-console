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
#ifndef QUEUE_H
#define QUEUE_H

#include "clock.h"

typedef void (*eventhandler_fn) (gametime_t, void *);

void queue_add_event(gametime_t, eventhandler_fn, void *);
void queue_remove_event(eventhandler_fn);
void queue_run_until(gametime_t);
gametime_t queue_get_first_time(void);
void queue_free(void);

#endif /* end of include guard: QUEUE_H */
