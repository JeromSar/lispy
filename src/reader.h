#ifndef READER_H
#define READER_H

#include "mpc/mpc.h"
#include "lval.h"
#include "symtable.h"

// Reader
void reader_set_filename(char* filename);
void reader_set_symtable(symtable* st);
lval* reader_read(mpc_ast_t* t);

// Utilities
int ast_count_nodes(mpc_ast_t* t);
int ast_count_leaves(mpc_ast_t* t);
int ast_count_branches(mpc_ast_t* t);

#endif
