#ifndef EVOLUTION__H_
#define EVOLUTION__H_

#include "tree.h"
#include <stdlib.h>
#include <stdbool.h>

void 	EVO_crossover(treenode_t *child,treenode_t *parent1, treenode_t *parent2);
void 	EVO_mutate(treenode_t *tree);
float 	EVO_fitness(treenode_t *tree, bool render);
void 	EVO_get_random_genome(char *genome, int maxlen);
void 	EVO_draw_tree(treenode_t *tree);
void 	EVO_sort_by_fitness(float *fitness, treenode_t **trees);
void 	EVO_crossover_on_generation(treenode_t **dest, treenode_t **src);

#endif
