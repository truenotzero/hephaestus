#include "window.h"

#include <stdlib.h>

struct windows {
  WINDOW *win;
  update_t pf_upd;
  refresh_t pf_ref;
  struct windows *next;
};

struct windows *windows_add(struct windows *ll, WINDOW *w, update_t u,
                            refresh_t r) {
  struct windows *newwin = malloc(sizeof(*newwin));
  newwin->win = w;
  newwin->pf_upd = u;
  newwin->pf_ref = r;
  newwin->next = ll;

  return ll;
}

void windows_update(struct windows *ll) {
  do {
    if (ll->pf_upd) {
      ll->pf_upd(ll->win);
    }
  } while ((ll = ll->next) != 0);
}

void windows_refresh(struct windows *ll) {
  erase();
  refresh();
  do {
    if (ll->pf_ref) {
      ll->pf_ref(ll->win);
    }
    wrefresh(ll->win);
  } while ((ll = ll->next) != 0);
}
void windows_free_all(struct windows *ll) {
  struct windows *temp;
  while (ll) {
    temp = ll;
    delwin(ll->win);
    ll = ll->next;
    free(temp);
  }
}
