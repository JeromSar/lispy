#include "stack.h"

CHEAT_TEST(stack_new,
  stack* stack = stack_new();

  cheat_assert(stack != NULL);
  cheat_assert_int(stack->len, 0);

  stack_del(stack);
)

CHEAT_TEST(stack_push_raw,
  stack* stack = stack_new();

  stack_push_raw(stack, "test", 1, 2);
  
  cheat_assert_int(stack->len, 1);
  cheat_assert_string(stack->entries[0]->sym, "test");
  cheat_assert_int(stack->entries[0]->row, 1);
  cheat_assert_int(stack->entries[0]->col, 2);

  stack_del(stack);
)

CHEAT_TEST(stack_pop,
  stack* stack = stack_new();

  stack_push_raw(stack, "test", 1, 2);
  stack_pop(stack);

  cheat_assert_int(stack->len, 0);

  stack_del(stack);
)

CHEAT_TEST(stack_peek,
  stack* stack = stack_new();

  stack_push_raw(stack, "test", 1, 2);
  stack_entry* se = stack_peek(stack);
  
  cheat_assert_int(stack->len, 1);
  cheat_assert_string(se->sym, "test");
  cheat_assert_int(se->row, 1);
  cheat_assert_int(se->col, 2);

  stack_del(stack);
)

CHEAT_TEST(stack_push_multiple,
  stack* stack = stack_new();
  for (int i = 1; i <= 200; i++) {
    stack_push_raw(stack, "test", i, 42);
  }

  cheat_assert_int(stack->len, 200);

  for (int i = 200; i >= 1; i--) {
    cheat_assert_int(stack_peek(stack)->row, i);
    stack_pop(stack);
  }
  stack_del(stack);
)
