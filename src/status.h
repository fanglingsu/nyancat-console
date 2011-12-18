#ifndef STATUS_H
#define STATUS_H

#include "main.h"

void status_set_mode(const enum gamemode mode);
void status_set_runtime(const int runtime);
void status_set_frames(const int frames);
void status_print(void);

#endif /* end of include guard: STATUS_H */
