#ifndef STACK_H
#define STACK_H

typedef struct sym_loc sym_loc;
typedef struct symtable symtable;

symtable* symtable_new(void);

void symtable_del(symtable* st);

int symtable_push_sym(symtable* st, char* str, int row, int col);

sym_loc* symtable_lookup_sym(symtable* st, int key);

void symtable_del_sym(symtable* st, int key);

#endif
