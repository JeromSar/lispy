#ifndef SYMTABLE_H
#define SYMTABLE_H

typedef struct sym_loc sym_loc;
struct sym_loc {
  char* filename; // owned
  int row;
  int col;
};

typedef struct sym_key_node sym_key_node;
struct sym_key_node {
  int key;
  sym_key_node* next;
};

typedef struct symtable symtable;
struct symtable {
  int len;
  int cap;
  sym_loc** syms; // owned, owned
  sym_key_node* free_syms;
};


symtable* symtable_new(void);

void symtable_del(symtable* st);

int symtable_push_sym(symtable* st, char* str, int row, int col);

sym_loc* symtable_lookup_sym(symtable* st, int key);

void symtable_del_sym(symtable* st, int key);

#endif
