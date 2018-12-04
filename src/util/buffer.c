#include <stdlib.h> // malloc, realloc
#include <stdarg.h> // va_list

#include "buffer.h"

static buffer bstdout = {
    .len = -1,
    .buf = NULL
};

buffer* buffer_stdout() {
    return &bstdout;
}

buffer* buffer_new(int cap) {
    // Min of 1
    cap = cap > 1 ? cap : 1;

    buffer* b = malloc(sizeof(buffer));
    b->len = 0;
    b->cap = cap;
    b->buf = malloc(sizeof(char) * cap);
    b->buf[0] = '\0';
    return b;
}

void buffer_del(buffer* b) {
    if (b == &bstdout) {
        return;
    }

    free(b->buf);
    free(b);
}

int buffer_printf(buffer* b, const char *format, ...) {
    int ret;
    va_list varg;
    va_start(varg, format);
    if (b == &bstdout) {
        // https://stackoverflow.com/questions/150543/forward-an-invocation-of-a-variadic-function-in-c
        ret = vprintf(format, varg);
        va_end(varg);
        return ret;
    }

    // https://stackoverflow.com/questions/3919995/determining-sprintf-buffer-size-whats-the-standard
    int size = vsnprintf(NULL, 0, format, varg);
    buffer_ensure(b, size + 1);
    ret = vsprintf(b->buf + b->len, format, varg);
    
    // Move the buffer over
    b->len += ret;

    va_end(varg);
    return ret;
}

void buffer_putchar(buffer* b, char c) {
    if (b == &bstdout) {
        putchar(c);
    }
    
    buffer_ensure(b, 2);

    b->buf[b->len] = c;
    b->buf[b->len + 1] = '\0';
    b->len += 1;
}

void buffer_ensure(buffer* b, int amt) {
    // Resizes the buffer so that there are at least 'amt' bytes available.

    if (b == &bstdout) {
        return;
    }

    // We can calculate the amount of bytes available: capacity - length
    if (b->cap - b->len >= amt) {
        return;
    }

    int new_cap = b->cap;
    while (new_cap - b->len < amt) {
        new_cap *= 2;
    }

    b->buf = realloc(b->buf, new_cap);
    b->cap = new_cap;
}

void buffer_shrink(buffer* b) {
    if (b == &bstdout) {
        return;
    }

    b->buf = realloc(b->buf, b->len + 1);
    b->cap = b->len + 1;
}