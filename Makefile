
run: a.out
	./$^ 2>log.log

a.out: editor.c buf.c main.c ui/menu.c ui/menubar.c ui/window.c
	clang -std=c17 $^ -Wall -Wextra -Wpedantic --debug -lncurses

clean:
	rm -f a.out

bear: clean
	bear make
