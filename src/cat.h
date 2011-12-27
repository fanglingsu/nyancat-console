#ifndef CAT_H
#define CAT_H

void cat_init(void);
void cat_jump_up(gametime_t);
void cat_jump_down(void);
void cat_run_handler(gametime_t time, void *data);
void cat_jump_handler(gametime_t time, void *data);
void cat_print(void);

#endif /* end of include guard: CAT_H */
