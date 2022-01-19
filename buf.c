#include "buf.h"
#include <stdlib.h>

// #undef max
// static inline int max(int a, int b) {
//   a -= b;
//   a &= (~a) >> 31;
//   a += b;
//   return a;
// }

struct buf {
  // The size of the dynamic buffer
  size_t sz;
  // Max number of elements before the buffer reallocates
  size_t capacity;
  char data[];
};

struct buf *buf_alloc(int sz) {
  size_t allocsz = sz < 1024 ? 1024 : sz;
  struct buf *ret = malloc(sizeof(*ret) + 2 * allocsz * sizeof(*ret->data));
  ret->sz = sz;
  ret->capacity = 2 * allocsz;
  return ret;
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

size_t buf_read(struct buf const *b, size_t pos, size_t cnt, char *tgt) {
  // check runoff
  if (pos + cnt > b->sz) {
    cnt = b->sz - pos;
  };

  for (size_t i = 0; i < cnt; ++i) {
    tgt[i] = b->data[pos + i];
  }

  return cnt;
}

void buf_write(struct buf **pb, size_t pos, size_t cnt, char const *tgt) {
  // check if realloc is needed
  struct buf *b = *pb;
  while (pos + cnt > b->capacity) {
    // reallocation needed
    buf_realloc(pb);
    b = *pb;
  }

  for (size_t i = 0; i < cnt; ++i) {
    b->data[pos + i] = tgt[i];
  }

  b->sz += cnt;
}

size_t buf_sz(struct buf const *e) { return e->sz; }
