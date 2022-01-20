#ifndef MENUBAR_H_
#define MENUBAR_H_

#include <curses.h>
#include <stddef.h>
#include <string.h>

#include "menu.h"

struct menubar {
	WINDOW* _menu;

  size_t padding;
  size_t ydrop;

  size_t active_menu_item;
  size_t active_menu;
  size_t num_menus;
  struct menu menus[];
};

/*
 * Allocate a new menubar. Do not use this function directly!
 * Use the `MENUBAR_CREATE_START()` macro instead
 */
struct menubar* menubar_alloc(size_t num_menus);

/*
 * Free the menubar and its resources
 */
void menubar_free(struct menubar*);

/*
 * Refreshes the menubar on screen
 */
int menubar_refresh(struct menubar const*, WINDOW*);

/*
 * Updates the menubar and its underlying menus
 *
 * The following actions are performed
 * - Update menu `_xpos` according to other menus and padding
 */
void menubar_update(struct menubar*);

/*
 * Render the menubar
 */
void menubar_render(struct menubar*, WINDOW*);

/*
 * Handles menubar input
 */
char menubar_getch(struct menubar*, WINDOW*);

#define MENUBAR_CREATE_START(name) \
  struct menubar* name;            \
  {                                \
    struct menu menus[] = {
#define __MENUBAR_ARR(...) ((struct menuitem[]){__VA_ARGS__})
#define __MENUBAR_COUNT_ELEMS(...) \
  (sizeof(__MENUBAR_ARR(__VA_ARGS__)) / sizeof(*__MENUBAR_ARR(__VA_ARGS__)))
#define MENUBAR_ADD_MENU(name, hotkey, ...)                    \
  menu_alloc(name, hotkey, __MENUBAR_COUNT_ELEMS(__VA_ARGS__), \
	     __MENUBAR_ARR(__VA_ARGS__)),
#define MENUBAR_ADD_EMPTY_MENU(name, hotkey) menu_alloc(name, hotkey, 0, 0),
#define MENUBAR_CREATE_END(name)                        \
  }                                                     \
  ;                                                     \
  name = menubar_alloc(sizeof(menus) / sizeof(*menus)); \
  memcpy(name->menus, menus, sizeof(menus));            \
  menubar_update(name);                                 \
  }

#endif	// MENUBAR_H_
