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
#include "main.h"
#include "queue.h"
#include "util.h"

typedef struct event {
    struct event *next;
    gametime_t time;
    eventhandler_fn callback;
    void *data;
} event_t;

static event_t *queue = NULL;

/**
 * @time: time whene to start the event.
 * @callback: callback that handles the event.
 * @data: data that where given to the event handler.
 *
 * Adds a new event to the event queue.
 */
void queue_add_event(gametime_t time, eventhandler_fn callback, void *data)
{
    extern event_t *queue;
    event_t **eqp, *eq;

    eq = (event_t *)xmalloc(sizeof(event_t));
    eqp = &queue;
    while (*eqp && (*eqp)->time <= time) {
        eqp = &((*eqp)->next);
    }

    eq->next = *eqp;
    eq->time = time;
    eq->callback = callback;
    eq->data = data;

    *eqp = eq;
}

/**
 * @callback: function that calling event should be removed.
 *
 * Remove all events that would call given callback.
 */
void queue_remove_event(eventhandler_fn callback)
{
    extern event_t *queue;
    event_t **eqp;

    eqp = &queue;
    while (*eqp) {
        struct event *eq = *eqp;
        if (eq->callback == callback) {
            *eqp = (*eqp)->next;
            free(eq);
        } else {
            eqp = &((*eqp)->next);
        }
    }
}

/**
 * @time: game time until that all event queue entries should be called.
 *
 * Runs all callbacks from event queue that should start until given time.
 */
void queue_run_until(gametime_t time)
{
    extern event_t *queue;

    while (queue && queue->time <= clock_get_relative()) {
        event_t *eq = queue;
        queue = queue->next;

        eq->callback(eq->time, eq->data);
        free(eq);
    }
}

/**
 * Retreives the gametime_t of the firt queue entry.
 */
gametime_t queue_get_first_time(void)
{
    extern event_t *queue;

    if (NULL == queue) {
        return 0;
    }
    return queue->time;
}

/**
 * Free all items of the event queue.
 */
void queue_free(void)
{
    extern event_t *queue;

    if (NULL == queue) {
        return;
    }

    event_t *eq = queue;
    queue = NULL;
    while (eq) {
        event_t *old = eq;
        eq = old->next;
        free(old);
    }
}
