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
#ifndef GAMEMODE_H
#define GAMEMODE_H

#include "clock.h"

typedef struct gamemode gamemode_t;
typedef void (*callback) (void);
typedef void (*keyhandler) (gametime_t, const int);

gamemode_t* mode_create(const char *, callback, callback, callback, keyhandler);
void mode_exit(void);
void gamemode_enter(const gamemode_t *);
void gamemode_draw(void);
void gamemode_key(const int);
int gamemode_valid(void);
const char* mode_get_name(void);

#endif /* end of include guard: GAMEMODE_H */
