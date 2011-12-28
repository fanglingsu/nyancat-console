#ifndef MODES_H
#define MODES_H

#include "clock.h"

void intro_draw(void);
void intro_key_handler(gametime_t, const int);

void game_enter(void);
void game_leave(void);
void game_draw(void);
void game_key_handler(gametime_t, const int);

void pause_enter(void);
void pause_key_handler(gametime_t, const int);

void scores_draw(void);
void scores_key_handler(gametime_t, const int);

#endif /* end of include guard: MODES_H */
