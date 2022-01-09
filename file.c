#include "file.h"
#include <stdio.h>
#include <stdlib.h>

struct hexfile {
  FILE *f;
  char const *path;
};

struct hexfile *file_new() {
  struct hexfile *h = malloc(sizeof(*h));
  h->f = fnew return h;
}