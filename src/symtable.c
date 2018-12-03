#include <stdlib.h>
#include "util.h"
#include "symtable.h"

// printf
#include "stdio.h"

#define START_AMOUNT 64

void ensure_cap(symtable* st, int min_cap) {
  if (st->cap >= min_cap) {
    return;
  }
  
  // Determine new capacity
  int new_cap = st->cap;
  while (new_cap < min_cap) {
    new_cap = new_cap * 2;
  }
  
  st->syms = realloc(st->syms, new_cap * sizeof(sym_loc*));
  st->cap = new_cap;
}

symtable* symtable_new() {
  symtable* st = malloc(sizeof(symtable));
  st->len = 0;
  st->cap = START_AMOUNT;
  st->syms = calloc(START_AMOUNT, sizeof(sym_loc*));
  st->free_syms = NULL;
  return st;
}

void symtable_del(symtable* st) {
  // Delete all the entries
  for (int i = st->len; i > 0; i--) {
    symtable_del_sym(st, i);
  }
  
  // Clear the free symbols
  while (st->free_syms) {
    sym_key_node* tmp = st->free_syms->next;
    free(st->free_syms);
    st->free_syms = tmp;
  }

  free(st->syms);
  free(st);
}

int symtable_push_sym(symtable* st, char* str, int row, int col) {
  // New symbol entry
  sym_loc* sl = malloc(sizeof(sym_loc));
  sl->filename = strdup(str);
  sl->row = row;
  sl->col = col;

  // Do we have a free symbol left?
  if (st->free_syms) {
    int key = st->free_syms->key;
    st->free_syms = st->free_syms->next;
    st->syms[key - 1] = sl;
    return key;
  } else {
    // New key at the end of the array
    ensure_cap(st, st->len + 1);
    st->syms[st->len] = sl;
    st->len = st->len + 1;
    return st->len;
  }
}

sym_loc* symtable_lookup_sym(symtable* st, int key) {
  if (key > st->len || key <= 0) {
    return NULL;
  }

  return st->syms[key - 1];
}

void symtable_del_sym(symtable* st, int key) {
  if (key > st->len || key <= 0) {
    return;
  }

  // Delete the entry
  sym_loc* sl = st->syms[key - 1];

  if (!sl) {
    return;
  }

  free(sl->filename);
  free(sl);
  st->syms[key - 1] = NULL;

  if (st->len == key) {
    // This is was the last key, don't add it to the free-syms list
    st->len--;
    return;
  }
  
  // We have a gap in the list, add it to 'st->free-syms'
  sym_key_node* node = malloc(sizeof(sym_key_node));
  node->key = key;
  
  if (!st->free_syms) {
    node->next = NULL;
  } else {
    node->next = st->free_syms;
  }
  
  st->free_syms = node;
}
