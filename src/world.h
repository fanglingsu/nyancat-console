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
#ifndef WORLD_H
#define WORLD_H

#include "clock.h"
#include "main.h"

enum object_type {
    ObjectNone,     /* no real object - here as placeholder */
    ObjectPlatform,
    ObjectMilk,
    ObjectCandy,
    ObjectRandom
};

extern coordinate_t screen;

void world_init(void);
void world_move_screen_right(void);
void world_move_screen_to(const int, const int);
void world_print(void);
int world_has_platform_at(const int, const int);
enum object_type world_get_object_at(const int, const int, const int);

#endif /* end of include guard: WORLD_H */
