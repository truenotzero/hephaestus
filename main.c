#include <curses.h>
#include <stdio.h>

#include "ui/menubar.h"

void loop();

WINDOW *win, *hex, *text;
struct menubar* mb;

void toggle_hex_editor() {
  if (!hex) {
    int sy, sx, ey, ex;
    getbegyx(win, sy, sx);
    getmaxyx(win, ey, ex);

    // create window on the left half
    hex = newwin(ey - 2, ex / 2 - 1, sy + 1, sx + 1);
    box(hex, 0, 0);
    mvwprintw(hex, 0, 2, "Hex Editor");
		loop();
    fprintf(stderr, "Created Hex Editor window\n");
  } else {
    werase(hex);
    wrefresh(hex);
    delwin(hex);
		hex = 0;
  }
}

void toggle_text_editor() {
  if (!text) {
    int sy, sx, ey, ex;
    getbegyx(win, sy, sx);
    getmaxyx(win, ey, ex);

    // create window on the right half
    text = newwin(ey - 2, ex / 2 - 2, sy + 1, ex / 2 + 1);
    box(text, 0, 0);
    mvwprintw(text, 0, 2, "Text Editor");
		loop();
    fprintf(stderr, "Created Text Editor window\n");
  } else {
    werase(text);
    wrefresh(text);
    delwin(text);
		text = 0;
  }
}

void loop() {
    menubar_getch(mb, win);
		menubar_render(mb, win);
		if (hex) {
			//box(hex, 0, 0);
			touchwin(hex);
		}
		if (text) {
			//box(text, 0, 0);
			touchwin(text);
		}
		refresh();
}

int main() {
  initscr();
  noecho();
  curs_set(0);

  win = newwin(0, 0, 0, 0);
  keypad(win, true);
  box(win, 0, 0);

  MENUBAR_CREATE_START(mymenubar)
  MENUBAR_ADD_MENU("File", 'f', {"New", 0}, {"Open", 0}, {"Save", 0},
		   {"Save As", 0})
  MENUBAR_ADD_MENU("View", 'v', {"Hex Editor", &toggle_hex_editor},
		   {"Text Editor", &toggle_text_editor})
  MENUBAR_CREATE_END(mymenubar)
  menubar_refresh(mymenubar, win);
	mb = mymenubar;

  do {
		loop();
  } while (1);

  wgetch(win);
  endwin();
  return 0;
}

