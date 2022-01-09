#include "editor.h"
#include "buf.h"
#include <stdio.h>
#include <stdlib.h>

struct editor {
  struct buf *buf;
  char const *path;
};
