#ifndef QUEUE_H
#define QUEUE_H

#include "clock.h"

typedef void (*eventhandler_fn) (gametime_t, void *);

void queue_add_event(gametime_t time, eventhandler_fn callback, void *data);
void queue_remove_event(eventhandler_fn callback);
void queue_run_until(gametime_t time);
gametime_t queue_get_first_time(void);
void queue_free(void);

#endif /* end of include guard: QUEUE_H */
