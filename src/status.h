#ifndef STATUS_H
#define STATUS_H

#include "main.h"
#include "clock.h"

void status_set_mode(const char *mode);
void status_set_runtime(const gametime_t runtime);
void status_set_frames(const int frames);
void status_print(void);

#endif /* end of include guard: STATUS_H */
