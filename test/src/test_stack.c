#include "minunit.h"
#include "stack.h"

MU_TEST(test_new) {
  stack* stack = stack_new();
  mu_assert(stack != NULL, "stack should not be NULL");
}

MU_TEST_SUITE(suite_stack) {
  MU_RUN_TEST(test_new);
}
