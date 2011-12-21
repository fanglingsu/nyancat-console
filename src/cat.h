#ifndef CAT_H
#define CAT_H

void cat_init(void);
void cat_jump_up(game_time);
void cat_jump_down(void);
void cat_jump_handler(game_time time, void *);
void cat_print(void);

#endif /* end of include guard: CAT_H */
