#ifndef MENU_H_
#define MENU_H_

#include <stddef.h>

struct menuitem {
  char const* name;
  void (*action)();
};

struct menu {
  size_t _xpos;

  char const* name;
  char hotkey;

  size_t num_items;
  struct menuitem* items;
};

struct menu menu_alloc(char const* name, char hotkey, size_t num_items, struct menuitem items[]);
void menu_free(struct menu *);

#endif // MENU_H_
