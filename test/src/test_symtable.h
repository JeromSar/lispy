#include "symtable.h"

CHEAT_TEST(symtable_new,
  symtable* st = symtable_new();
  cheat_assert(st != NULL);
  symtable_del(st);
)

CHEAT_TEST(symtable_push_lookup,
  symtable* st = symtable_new();
  
  int ref = symtable_push_sym(st, "hello", 42, 33);
  cheat_assert_int(ref, 1);
  
  sym_loc* sym = symtable_lookup_sym(st, ref);
  
  cheat_assert(sym != NULL);
  cheat_assert_string(sym->filename, "hello");
  cheat_assert_int(sym->row, 42);
  cheat_assert_int(sym->col, 33);

  symtable_del(st);
)

CHEAT_TEST(symtable_push_del,
  symtable* st = symtable_new();
  
  int ref = symtable_push_sym(st, "hello", 42, 33);
  cheat_assert_int(ref, 1);
  
  sym_loc* sym = symtable_lookup_sym(st, ref);
  cheat_assert(sym != NULL);
  
  symtable_del_sym(st, ref);
  sym = symtable_lookup_sym(st, ref);
  cheat_assert(sym == NULL);

  symtable_del(st);
)

CHEAT_TEST(symtable_push_del_100,
  symtable* st = symtable_new();

  for (int i = 1; i <= 100; i++) {
    symtable_push_sym(st, "hello", i, 42);
  }
  
  cheat_assert_int(st->len, 100);
  cheat_assert_int(st->cap, 128);

  for (int i = 1; i <= 100; i++) {
    sym_loc* sym = symtable_lookup_sym(st, i);
    cheat_assert(sym != NULL);
    cheat_assert_int(sym->row, i);
  }

  symtable_del(st);
)

CHEAT_TEST(symtable_push_del_complex,
  symtable* st = symtable_new();

  for (int i = 1; i <= 100; i++) {
    symtable_push_sym(st, "hello", i, 42);
  }
  
  cheat_assert_int(st->len, 100);
  cheat_assert_int(st->cap, 128);
  cheat_assert(st->free_syms == NULL);
  
  // Delete and assert
  for (int i = 20; i <= 20 + 30; i += 3) {
     symtable_del_sym(st, i);
     cheat_assert(symtable_lookup_sym(st, i) == NULL);
  }
  
  // Check if they're added to the free list
  int free_amt = 0;
  sym_key_node* loop = st->free_syms;
  while (loop != NULL) {
    free_amt++;
    loop = loop->next;
  }
  cheat_assert_int(free_amt, 11);
  
  // Add a new node
  int ref1 = symtable_push_sym(st, "hi", 12, 42);
  cheat_assert_int(ref1, 50);
  free_amt = 0;
  loop = st->free_syms;
  while (loop != NULL) {
    free_amt++;
    loop = loop->next;
  }
  cheat_assert_int(free_amt, 10);
  
  // And another
  int ref2 = symtable_push_sym(st, "hi", 12, 42);
  cheat_assert_int(ref2, 47);
  free_amt = 0;
  loop = st->free_syms;
  while (loop != NULL) {
    free_amt++;
    loop = loop->next;
  }
  cheat_assert_int(free_amt, 9);

  symtable_del(st);
)
