#include "menubar.h"

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "menu.h"

void submenu(struct menubar*);

struct menubar* menubar_alloc(size_t num_menus) {
  struct menubar* ret = malloc(sizeof(*ret) + num_menus * sizeof(*ret->menus));
  ret->num_menus = num_menus;
  ret->padding = 1;
  ret->ydrop = 0;
  ret->active_menu_item = -1;
  ret->active_menu = -1;
  return ret;
}

void menubar_free(struct menubar* mb) {
  for (size_t i = 0; i < mb->num_menus; ++i) {
    menu_free(&mb->menus[i]);
  }
  free(mb);
}

void menubar_update(struct menubar* mb) {
  size_t lastx = mb->padding;
  for (size_t i = 0; i < mb->num_menus; ++i) {
    mb->menus[i]._xpos = lastx;
    lastx += strlen(mb->menus[i].name) + mb->padding;
  }
}

int menubar_refresh(struct menubar const* mb, WINDOW* w) {
  for (size_t i = 0; i < mb->num_menus; ++i) {
    struct menu const m = mb->menus[i];
    mvwprintw(w, mb->ydrop, m._xpos, m.name);
  }

  return 0;
}

void menubar_render(struct menubar* mb, WINDOW* w) {
  for (size_t i = 0; i < mb->num_menus; ++i) {
    struct menu const m = mb->menus[i];

    if (i == mb->active_menu) wattron(w, A_STANDOUT);
    mvwprintw(w, mb->ydrop, m._xpos, m.name);
    wattroff(w, A_STANDOUT);
  }

  wrefresh(w);

  if (mb->_menu) {
    struct menu const m = mb->menus[mb->active_menu];
    int mx = getmaxx(mb->_menu);
    for (size_t i = 0; i < m.num_items; ++i) {
      struct menuitem it = m.items[i];

      mvwprintw(mb->_menu, i, 0, it.name);
      int style = i == mb->active_menu_item ? A_STANDOUT : A_NORMAL;
      mvwchgat(mb->_menu, i, 0, mx, style, 0, 0);
    }

    wrefresh(mb->_menu);
  }
}

char menubar_getch(struct menubar* mb, WINDOW* w) {
  int c = wgetch(w);

  if (mb->active_menu < mb->num_menus) {
    switch (c) {
      case KEY_LEFT:
	if (mb->active_menu == 0) {
	  mb->active_menu = mb->num_menus - 1;
	} else {
	  --mb->active_menu;
	}
	break;

      case KEY_RIGHT:
	if (++mb->active_menu >= mb->num_menus) {
	  mb->active_menu -= mb->num_menus;
	}
	break;
      default:
	mb->active_menu = -1;
    }
  }

  for (size_t i = 0; i < mb->num_menus; ++i) {
    if (c == mb->menus[i].hotkey) {
      mb->active_menu = i;
    }
  }

  if (mb->active_menu < mb->num_menus) {
    struct menu const m = mb->menus[mb->active_menu];
    if (m.num_items) {
      submenu(mb);
      box(w, 0, 0);
      return c;
    }
  }

  ungetch(c);
  return c;
}

void submenu(struct menubar* mb) {
  struct menu const m = mb->menus[mb->active_menu];
  if (!mb->_menu) {
    int ysize = m.num_items;
    int xsize = strlen(m.name);
    for (size_t i = 0; i < m.num_items; ++i) {
      int newsize = strlen(m.items[i].name);
      if (newsize > xsize) {
	xsize = newsize;
      }
    }

    mb->_menu = newwin(ysize, xsize, 1, m._xpos);
    mb->active_menu_item = 0;
    keypad(mb->_menu, true);
  }

  int c = wgetch(mb->_menu);
  switch (c) {
    case KEY_UP:
      if (mb->active_menu_item == 0) {
	mb->active_menu_item = m.num_items - 1;
      } else {
	--mb->active_menu_item;
      }
      break;
    case KEY_DOWN:
      if (++mb->active_menu_item >= m.num_items) {
	mb->active_menu_item = 0;
      }
      break;
    case '\n':
      fprintf(stderr, "Enter hit!\n");
      void (*f)() = m.items[mb->active_menu_item].action;
      if (f) {
	f();
      }
      /* fall through */
    default:
      ungetch(c);
      werase(mb->_menu);
      wrefresh(mb->_menu);
      delwin(mb->_menu);
      mb->_menu = 0;
  }
}

