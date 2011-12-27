#ifndef MODES_H
#define MODES_H

void intro_draw(void);
void intro_key_handler(gametime_t time, const int key);

void game_enter(void);
void game_leave(void);
void game_draw(void);
void game_key_handler(gametime_t time, const int key);

void pause_enter(void);
void pause_key_handler(gametime_t time, const int key);

void scores_draw(void);
void scores_key_handler(gametime_t time, const int key);

#endif /* end of include guard: MODES_H */
