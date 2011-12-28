#ifndef WORLD_H
#define WORLD_H

#include "clock.h"

enum object_type {
    ObjectPlatform
};

void world_init(void);
void world_move_screen_right(const int);
void world_move_screen_to(const int, const int);
void world_print(void);
int world_has_element_at(enum object_type, const int, const int);

#endif /* end of include guard: WORLD_H */
