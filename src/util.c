#include <stdlib.h>
#include <string.h>

#include "util.h" // bool

long lpow(long x, long y) {
  while (y > 1) {
    x *= x;
    y--;
  }
  return x;
}

bool streq(const char* a, const char* b) {
  return strcmp(a, b) == 0;
}

char* stdup(const char* a) {
  int size = strlen(a);
  char* dup = malloc(size + 1);
  memcpy(dup, a, size + 1);
  return dup;
}
