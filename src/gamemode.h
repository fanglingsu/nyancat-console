#ifndef GAMEMODE_H
#define GAMEMODE_H

#include "clock.h"

typedef struct _mode gamemode_t;
typedef void (*callback) (void);
typedef void (*keyhandler) (gametime_t, const int);

gamemode_t* mode_create(const char *name, callback enter, callback leave, callback redraw, keyhandler key);
void mode_exit(void);
void gamemode_enter(const gamemode_t *mode);
void gamemode_draw(void);
void gamemode_key(const int key);
int gamemode_valid(void);
const char* mode_get_name(void);

#endif /* end of include guard: GAMEMODE_H */
