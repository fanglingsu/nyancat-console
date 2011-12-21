#ifndef QUEUE_H
#define QUEUE_H

#include "clock.h"

typedef void (*eventhandler) (game_time, void *);

void queue_add_event(game_time time, eventhandler callback, void *data);
void queue_run_until(game_time time);
game_time queue_get_first_time(void);
void queue_free(void);

#endif /* end of include guard: QUEUE_H */
