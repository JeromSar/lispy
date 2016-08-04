#include "mpc/mpc.h"

int ast_count_nodes(mpc_ast_t* t) {
  if (t->children_num == 0) { return 1; }
  if (t->children_num >= 1) {
    int total = 1;
    for (int i = 0; i < t->children_num; i++) {
      total += ast_count_nodes(t->children[i]);
    }
    return total;
  }
  return 0;
}

int ast_count_leaves(mpc_ast_t* t) {
  if (t->children_num == 0) { return 1; }
  if (t->children_num >= 1) {
    int total = 0;
    for (int i = 0; i < t->children_num; i++) {
      total += ast_count_leaves(t->children[i]);
    }
    return total;
  }
  return 0;
}

int ast_count_branches(mpc_ast_t* t) {
  if (t->children_num == 0) { return 0; }
  if (t->children_num >= 1) {
    int total = 1;
    for (int i = 0; i < t->children_num; i++) {
      total += ast_count_branches(t->children[i]);
    }
    return total;
  }
  return 0;
}

