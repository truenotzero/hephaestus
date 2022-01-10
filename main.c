#include "editor.h"

int main() {
  struct editor *e = editor_new();
  struct buf *b = editor_buf(e);
  char TEXT[] = "hello, world!\n";
  buf_write(&b, 0, sizeof(TEXT) / sizeof(*TEXT), TEXT);
  editor_save_as(e, "test.hex");
  editor_close(e);
  return 0;
}