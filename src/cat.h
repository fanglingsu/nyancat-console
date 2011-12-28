#ifndef CAT_H
#define CAT_H

#include "clock.h"

void cat_init(void);
void cat_jump_up(gametime_t);
void cat_jump_down(void);
int cat_get_height(void);
void cat_move_right(const int);
void cat_move_handler(gametime_t, void *);
void cat_print(void);

#endif /* end of include guard: CAT_H */
