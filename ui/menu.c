#include "menu.h"

#include <stdlib.h>
#include <string.h>

struct menu menu_alloc(char const* name, char hotkey, size_t num_items, struct menuitem items[]) {
	struct menu ret = {
		.name = name,
		.hotkey = hotkey,
		.num_items = num_items,
		.items = malloc(num_items * sizeof(*ret.items)),
	};

	for (size_t i = 0; i < num_items; ++i) {
		ret.items[i].action = items[i].action;
		ret.items[i].name = items[i].name;
	}

	return ret;
}

void menu_free(struct menu *m) {
	free(m->items);
}

