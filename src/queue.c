#include <stdlib.h>
#include "main.h"
#include "queue.h"
#include "clock.h"

struct _event {
    struct _event *next;
    gametime_t time;
    eventhandler_fn callback;
    void *data;
};

static struct _event *queue = NULL;

/**
 * @time: time whene to start the event.
 * @callback: callback that handles the event.
 * @data: data that where given to the event handler.
 *
 * Adds a new event to the event queue.
 */
void
queue_add_event(gametime_t time, eventhandler_fn callback, void *data)
{
    struct _event **eqp, *eq;

    eq = (struct _event *)malloc(sizeof(struct _event));
    if (NULL == eq) {
        error_exit("Could not allocate memory");
    }
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
void
queue_remove_event(eventhandler_fn callback)
{
    struct _event **eqp;

    eqp = &queue;
    while (*eqp) {
        struct _event *eq = *eqp;
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
void
queue_run_until(gametime_t time)
{
    while (queue && queue->time <= clock_get_relative()) {
        struct _event *eq = queue;
        queue = queue->next;

        eq->callback(eq->time, eq->data);
        free(eq);
    }
}

/**
 * Retreives the gametime_t of the firt queue entry.
 */
gametime_t
queue_get_first_time(void)
{
    if (NULL == queue) {
        return 0;
    }
    return queue->time;
}

/**
 * Free all items of the event queue.
 */
void
queue_free(void)
{
    if (NULL == queue) {
        return;
    }

    struct _event *eq = queue;
    queue = NULL;
    while (eq) {
        struct _event *old = eq;
        eq = old->next;
        free(old);
    }
}
