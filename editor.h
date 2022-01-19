#ifndef EDITOR_H_
#define EDITOR_H_

#include "buf.h"

struct editor;

/*
 * Create a new default hex editor
 */
struct editor *editor_new();

/*
 * Open a file on disk for editing
 * path - The path to the file on disk
 */
struct editor *editor_open(char const *path);

/*
 * Save the current editor
 * Returns: 0 for success
 */
int editor_save(struct editor const *);

/*
 * Save the current editor at the specified path
 * Creates the file if it doesn not exist, overwrites it if it does
 * path- The path to the file on disk
 * Returns: see `editor_close`
 */
int editor_save_as(struct editor *, char const *path);

/*
 * Close the editor, freeing up resources
 * Returns: 0 for success
 */
int editor_close(struct editor *);

/*
 * Get the editor's buffer to manipulate text
 */
struct buf *editor_buf(struct editor *);

#endif // EDITOR_H_
