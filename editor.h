#ifndef EDITOR_H_
#define EDITOR_H_

struct editor;

struct editor *editor_new();
struct editor *editor_open(char const *path);
int editor_save(struct editor *const);
int editor_save_as(struct editor *const, char const *path);
int editor_close(struct editor *);

#endif // EDITOR_H_