#include "evolution.h"



void EVO_crossover(tree *parent1, tree *parent2, tree *child)
{
	int coin;

	// left
	if ((parent1->left != NULL) && (parent2->left != NULL)) {
		// allocate branch for children
		tree_init(&(child->left));
		EVO_crossover(parent1->left, parent2->left, child->left);
	} else {
		// flip coin
		coin = rand() % 2;

		// first parent
		if (coin) {
			tree_copy(child, parent1);
		} else {
			tree_copy(child, parent2);
		}
	}

	// right

	// up
}
