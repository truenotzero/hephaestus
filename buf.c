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
  return buf_own(0, sz ? sz : default_size);
}

struct buf *buf_own(void *b, int sz) {
  struct buf *ret = realloc(b, sizeof(*ret) + 2 * sz * sizeof(*ret->data));
  ret->sz = sz;
  ret->capacity = 2 * sz;
  return ret;
}

void buf_realloc(struct buf **b) {
  *b = realloc(*b, sizeof(**b) + 2 * (*b)->capacity * sizeof(*(*b)->data));
  (*b)->capacity *= 2;
}

void buf_free(struct buf *b) { free(b); }

int buf_read(struct buf const *b, int pos, int cnt, char *tgt) {
  // check runoff
  if (pos + cnt > b->sz) {
    cnt = b->sz - pos;
  };

  for (int i = 0; i < cnt; ++i) {
    tgt[i] = b->data[pos + i];
  }

  return cnt;
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

int buf_sz(struct buf const *e) { return e->sz; }