#include "lval.h"
#include "util/buffer.h"

CHEAT_TEST(lval_str_print,
    buffer* b = buffer_new(16);
    lval* s = lval_str("Hello, world!");
    
    lval_print(b, s);

    cheat_assert_string(b->buf, "\"Hello, world!\"");

    lval_del(s);
    buffer_del(b);
)

CHEAT_TEST(lval_long_print,
    buffer* b = buffer_new(16);
    lval* s = lval_long(51624);
    
    lval_print(b, s);

    cheat_assert_string(b->buf, "51624");

    lval_del(s);
    buffer_del(b);
)

CHEAT_TEST(lval_fun_print,
    buffer* b = buffer_new(16);
    
    // Create function: (\ {x y} {+ x y})
    lval* formals = lval_qexpr();
    lval_add(formals, lval_sym("x"));
    lval_add(formals, lval_sym("y"));

    lval* body = lval_qexpr();
    lval_add(body, lval_sym("+"));
    lval_add(body, lval_sym("x"));
    lval_add(body, lval_sym("y"));

    lval* fun = lval_lambda(formals, body);

    lval_print(b, fun);

    cheat_assert_string(b->buf, "(\\ {x y} {+ x y})");

    lval_del(fun);
    buffer_del(b);
)

// TODO: Others..
