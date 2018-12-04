#include "util/buffer.h"

CHEAT_TEST(buffer_new,
  buffer* b = buffer_new(16);

  cheat_assert(b != NULL);
  cheat_assert_int(b->len, 0);
  cheat_assert_int(b->cap, 16);
  cheat_assert(b->buf[0] == '\0');

  buffer_del(b);
)

CHEAT_TEST(buffer_printf,
    buffer* b = buffer_new(16);

    buffer_printf(b, "abc");

    cheat_assert_int(b->len, 3);
    cheat_assert_int(b->cap, 16);
    cheat_assert_string(b->buf, "abc");

    buffer_del(b);
)

CHEAT_TEST(buffer_printf_format,
    buffer* b = buffer_new(16);

    buffer_printf(b, "abc: %d", 123);

    cheat_assert_int(b->len, 8);
    cheat_assert_int(b->cap, 16);
    cheat_assert_string(b->buf, "abc: 123");

    buffer_del(b);
)

CHEAT_TEST(buffer_printf_ensure,
    buffer* b = buffer_new(16);

    buffer_printf(b, "abcdefghijklmnopqrstuvwxyz");

    cheat_assert_int(b->len, 26);
    cheat_assert_int(b->cap, 32);
    cheat_assert_string(b->buf, "abcdefghijklmnopqrstuvwxyz");

    buffer_del(b);
)


CHEAT_TEST(buffer_printf_ensure_len,
    buffer* b = buffer_new(16);

    buffer_printf(b, "123456789012345");

    cheat_assert_int(b->len, 15);
    cheat_assert_int(b->cap, 16);
    cheat_assert_string(b->buf, "123456789012345");

    buffer_printf(b, "a");

    cheat_assert_int(b->len, 16);
    cheat_assert_int(b->cap, 32);
    cheat_assert_string(b->buf, "123456789012345a");

    buffer_del(b);
)

CHEAT_TEST(buffer_putchar,
    buffer* b = buffer_new(2);

    buffer_putchar(b, 'a');

    cheat_assert_int(b->len, 1);
    cheat_assert_int(b->cap, 2);
    cheat_assert_string(b->buf, "a");

    buffer_putchar(b, 'b');

    cheat_assert_int(b->len, 2);
    cheat_assert_int(b->cap, 4);
    cheat_assert_string(b->buf, "ab");

    buffer_putchar(b, 'c');

    cheat_assert_int(b->len, 3);
    cheat_assert_int(b->cap, 4);
    cheat_assert_string(b->buf, "abc");

    buffer_del(b);
)

CHEAT_TEST(buffer_ensure_low,
    buffer* b = buffer_new(16);

    buffer_ensure(b, 8);

    cheat_assert_int(b->cap, 16);

    buffer_del(b);
)

CHEAT_TEST(buffer_ensure_boundary_under,
    buffer* b = buffer_new(16);

    buffer_ensure(b, 16);

    cheat_assert_int(b->cap, 16);

    buffer_del(b);
)

CHEAT_TEST(buffer_ensure_boundary_over,
    buffer* b = buffer_new(16);

    buffer_ensure(b, 17);

    cheat_assert_int(b->cap, 32);

    buffer_del(b);
)


CHEAT_TEST(buffer_ensure_high,
    buffer* b = buffer_new(16);

    buffer_ensure(b, 20);

    cheat_assert_int(b->cap, 32);

    buffer_del(b);
)

CHEAT_TEST(buffer_ensure_very_high,
    buffer* b = buffer_new(16);

    buffer_ensure(b, 135);

    cheat_assert_int(b->cap, 256);

    buffer_del(b);
)

CHEAT_TEST(buffer_shrink,
    buffer* b = buffer_new(128);

    buffer_printf(b, "abcd");

    buffer_shrink(b);

    cheat_assert_int(b->cap, 5);

    buffer_del(b);
)