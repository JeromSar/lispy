#include "stack.h"

CHEAT_TEST(stack_new,
  stack* stack = stack_new();
  cheat_assert(stack != NULL);
  cheat_assert_int(stack->len, 0);
  stack_del(stack);
)

CHEAT_TEST(stack_push_pop,
  stack* stack = stack_new();
  stack_push(stack, 42);
  cheat_assert_int(stack->len, 1);
  cheat_assert_int(stack_pop(stack), 42);
  stack_del(stack);
)

CHEAT_TEST(stack_push_multiple,
  stack* stack = stack_new();
  for (int i = 1; i <= 200; i++) {
    stack_push(stack, i);
  }

  cheat_assert_int(stack->len, 200);

  for (int i = 200; i >= 1; i--) {
    cheat_assert_int(stack_pop(stack), i);
  }
  stack_del(stack);
)
