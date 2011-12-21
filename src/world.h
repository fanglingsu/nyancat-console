#ifndef WORLD_H
#define WORLD_H

#include "clock.h"

void world_init(void);
void world_scroll_handler(game_time time, void *data);
void world_start_scrolling(void);
void world_print(void);

#endif /* end of include guard: WORLD_H */
