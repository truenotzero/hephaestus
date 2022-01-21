#ifndef WINDOW_H_
#define WINDOW_H_

#include <curses.h>

typedef void (*update_t)(WINDOW *);
typedef void (*refresh_t)(WINDOW *);

struct windows;

struct windows *windows_add(struct windows *, WINDOW *, update_t, refresh_t);
void windows_update(struct windows *);
void windows_refresh(struct windows *);
void windows_free_all(struct windows *);

#endif // WINDOW_H_
