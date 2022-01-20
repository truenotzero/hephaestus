#include "editor.h"
#include <stdio.h>
#include <stdlib.h>

struct editor {
  struct buf *buf;
  char const *path;
};

struct editor *editor_new() {
  struct editor *e = malloc(sizeof(*e));
  e->buf = buf_alloc(0);
  e->path = 0;
  return e;
}

int editor_close(struct editor *e) {
  buf_free(e->buf);
  free(e);
  return 0;
}

struct editor *editor_open(char const *path) {
  struct editor *e = editor_new();
  e->path = path;

  FILE *f = fopen(path, "rb");
  if (!f) {
    // failed to open
    editor_close(e);
    return 0;
  }

  // get file length
  fseek(f, 0, SEEK_END);
  int len = ftell(f);

  // allocate temp buffer and fill it
  char *temp = malloc(len * sizeof(*temp));
  fread(temp, sizeof(*temp), len, f);

  // copy from temp buf to editor->buf
  struct buf *b = e->buf;
  buf_write(&b, 0, len, temp);

  // cleanup and return
  free(temp);
  fclose(f);
  return e;
}

int editor_save(struct editor const *e) {
  FILE *f = fopen(e->path, "wb");

  if (!f) {
    // failed to open
    return 1;
  }

  // copy editor->buf to temp buf
  // cast away `const` legal - read only access
  struct buf const *b = editor_buf((struct editor *)e);
  size_t sz = buf_sz(b);
  char *temp = malloc(sz * sizeof(*temp));
  if (sz != buf_read(b, 0, sz, temp)) {
    // discrepancy between bytes read and buffer
    return 2;
  }
  // write to disk
  if (sz != fwrite(temp, sizeof(*temp), sz, f)) {
    // discrepancy between bytes written to disk and elems in temp
    return 3;
  }

  // clenaup and return
  free(temp);
  fclose(f);
  return 0;
}

int editor_save_as(struct editor *e, char const *path) {
  e->path = path;
  return editor_save(e);
}

struct buf *editor_buf(struct editor *e) {
  return e->buf;
}
