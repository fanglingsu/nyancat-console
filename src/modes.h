#ifndef MODES_H
#define MODES_H

void intro_draw(void);
void intro_key_handler(game_time time, const int key);

void game_enter(void);
void game_draw(void);
void game_key_handler(game_time time, const int key);

void pause_enter(void);
void pause_leave(void);
void pause_key_handler(game_time time, const int key);

void scores_draw(void);
void scores_key_handler(game_time time, const int key);

#endif /* end of include guard: MODES_H */
