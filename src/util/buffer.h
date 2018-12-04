#ifndef BUFFER_H
#define BUFFER_H

typedef struct buffer buffer;

#include <stdio.h>

struct buffer {
    int len;
    int cap;
    char* buf;
};

buffer* buffer_stdout();
buffer* buffer_new(int cap);
void buffer_del(buffer* b);

int buffer_printf(buffer* b, const char *format, ...);
void buffer_putchar(buffer* b, char c);

void buffer_shrink(buffer* b);
void buffer_ensure(buffer* b, int amt);

#endif
