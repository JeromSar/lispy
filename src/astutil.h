#ifndef ASTUTIL_H
#define ASTUTIL_H

#include "mpc/mpc.h"

int ast_count_nodes(mpc_ast_t* t);
int ast_count_leaves(mpc_ast_t* t);
int ast_count_branches(mpc_ast_t* t);

#endif