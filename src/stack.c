#include<stdlib.h>
#include "stack.h"

// TODO: debug statement
#include <stdio.h>

stack* stack_new() {
  stack* st = malloc(sizeof(stack));
  st->len = 0;
  st->syms = malloc(STACK_SIZE * sizeof(int));
  return st;
}

bool stack_push(stack* st, int sym) {
  if (st->len == STACK_SIZE) {
    return false;
  }
  st->syms[st->len] = sym;
  st->len++;
  printf("Stack push: %d\n", sym);
  return true;
}

int stack_pop(stack* st) {
  if (st->len == 0) {
    return -1;
  }
  st->len--;
  printf("Stack pop: %d\n", st->syms[st->len]);
  return st->syms[st->len];
}

void stack_del(stack* st) {
  free(st->syms);
  free(st);
}
