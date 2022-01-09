#include "buf.h"
#include <stdlib.h>

#undef max
static inline int max(int a, int b) {
  a -= b;
  a &= (~a) >> 31;
  a += b;
  return a;
}

struct buf {
  // The size of the dynamic buffer
  int sz;
  // Max number of elements before the buffer reallocates
  int capacity;
  char data[];
};

struct buf *buf_alloc(int sz) {
  const int default_size = 1024;
  struct buf *b;
  if (!sz) {
    // Create default buffer
    b = malloc(sizeof(*b) + default_size * sizeof(*b->data));
    b->sz = 0;
    b->capacity = default_size;
  } else {
    // Create buffer with size
    b = calloc(1, sizeof(*b) + 2 * sz * sizeof(*b->data));
    b->sz = sz;
    b->capacity = 2 * sz;
  }
  return b;
}

void buf_realloc(struct buf **b) {
  struct buf *ob = *b;
  struct buf *nb = malloc(sizeof(*nb) + 2 * ob->capacity * sizeof(*nb->data));
  nb->sz = ob->sz;
  nb->capacity = 2 * ob->capacity;
  free(ob);
  *b = nb;
}

void buf_free(struct buf *b) { free(b); }

int buf_read(struct buf const *b, int pos, int cnt, char *tgt) {
  // check runoff
  if (pos + cnt > b->sz) {
    cnt = b->sz - pos;
  };

  int i;
  for (i = 0; i < cnt; ++i) {
    tgt[i] = b->data[pos + i];
  }

  return i;
}

void buf_write(struct buf **pb, int pos, int cnt, char const *tgt) {
  // check if realloc is needed
  struct buf *b = *pb;
  while (pos + cnt > b->capacity) {
    // reallocation needed
    buf_realloc(pb);
    b = *pb;
  }

  for (int i = 0; i < cnt; ++i) {
    b->data[pos + i] = tgt[i];
  }

  b->sz += cnt;
}