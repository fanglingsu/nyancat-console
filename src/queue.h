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
