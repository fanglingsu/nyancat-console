#ifndef WORLD_H
#define WORLD_H

#include "clock.h"

enum object_type {
    ObjectPlatform
};

void world_init(void);
void world_scroll_handler(gametime_t time, void *data);
void world_start_scrolling(void);
void world_print(void);
int world_has_element_at(enum object_type type, const int y, const int x);

#endif /* end of include guard: WORLD_H */
