#include "minunit.h"
#include <test_stack.c>

int main(int argc, const char** argv) {
  MU_RUN_SUITE(suite_stack);
  MU_REPORT();
  return 0;
}
