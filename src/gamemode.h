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
