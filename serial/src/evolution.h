#ifndef EVOLUTION__H_
#define EVOLUTION__H_

#include "tree.h"
#include <stdlib.h>

void EVO_crossover(treenode_t *child,treenode_t *parent1, treenode_t *parent2);
void EVO_mutate(treenode_t *tree);

#endif
