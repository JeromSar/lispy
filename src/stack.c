#include <stdlib.h> // malloc
#include <stdio.h> // printf

#include "util/util.h" // strdup
#include "stack.h"

static stack_entry UNKNOWN_ENTRY = {
  .sym = "<unknown!>",
  .row = 0,
  .col = 0
};

stack* stack_new() {
  stack* st = malloc(sizeof(stack));
  st->len = 0;
  st->entries = malloc(STACK_SIZE * sizeof(stack_entry*));
  return st;
}

void stack_del(stack* st) {
  
  // Delete all entries
  while (st->len > 0) {
    stack_pop(st);
  }

  free(st->entries);
  free(st);
}

stack* stack_copy(stack* st) {
  stack* copy = stack_new();

  for (int i = 0; i < st->len; i++) {
    stack_entry* se = st->entries[i];
    stack_push_raw(copy, se->sym, se->row, se->col);
  }

  return copy;
}

bool stack_push_raw(stack* st, char* sym, int row, int col) {
  if (st->len >= STACK_SIZE) {
    return false;
  }
  
  stack_entry* se;
  if (sym != NULL) {
    // Make new stack entry
    se = malloc(sizeof(stack_entry));
    se->sym = strdup(sym);
    se->row = row;
    se->col = col;  
  } else {
    se = &UNKNOWN_ENTRY;
  }
  
  // Insert entry
  st->entries[st->len] = se;
  st->len++;
  return true;
}

bool stack_push(stack* st, sym_loc* loc) {
  if (loc != NULL) {
    return stack_push_raw(st, loc->filename, loc->row, loc->col);
  } else {
    return stack_push_raw(st, NULL, 0, 0); 
  }
}

bool stack_push_lval(stack* st, lcontext* ctx, lval* val) {
  if (!val->loc) {
    printf("stack_push_lval: lval has no location\n", val->loc);
    return stack_push(ctx->stack, NULL);
  }
  
  sym_loc* sl = symtable_lookup_sym(ctx->symtable, val->loc);
  if (sl == NULL) {
    printf("stack_push_lval: Symkey %d resolves to NULL\n", val->loc);
    return stack_push(ctx->stack, NULL);
  }

  return stack_push(ctx->stack, sl);
}

bool stack_pop(stack* st) {
  if (st->len == 0) {
    return true;
  }
  
  // TODO: Improve performance, don't delete stack entries
  // Delete stack entry
  stack_entry* se = st->entries[st->len-1];
  
  if (se != &UNKNOWN_ENTRY) {
    free(se->sym);
    free(se);
  }
  st->entries[st->len-1] = NULL;

  st->len--;
  return false;
}

stack_entry* stack_peek(stack* st) {
  if (st->len == 0) {
    return NULL;
  }
  
  return st->entries[st->len - 1];
}

void stack_print(stack* st, buffer* b) {
  for (int i = st->len-1; i >= 0; i--) {
    stack_entry* se = st->entries[i];
    if (se->row == 0 && se->col == 0) {
      printf("\tat %s\n",
        se->sym
      );
    } else {
      buffer_printf(b, "\tat %s:%d:%d\n",
        se->sym,
        se->row,
        se->col
      );
    }
  }
}
