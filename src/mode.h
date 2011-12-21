#ifndef MODE_H
#define MODE_H

typedef struct _mode Mode;
typedef void (*callback) (void);
typedef void (*keyhandler) (const int);

Mode* mode_create(const char *name, callback enter, callback leave, callback redraw, keyhandler key);
void mode_exit(void);
void mode_enter(const Mode *mode);
void mode_draw(void);
void mode_key(const int key);
int mode_valid(void);
const char* mode_get_name(void);

#endif /* end of include guard: MODE_H */
