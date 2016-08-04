#include <string.h>

#include "util.h" // bool

long lpow(long x, long y) {
  while (y > 1) {
    x *= x;
    y--;
  }
  return x;
}

bool streq(char* a, char* b) {
  return strcmp(a, b) == 0;
}