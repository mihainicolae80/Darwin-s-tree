#include "evolution.h"



void EVO_crossover(treenode_t *child, treenode_t *parent1, treenode_t *parent2)
{
	int coin;

	// left
	if ((parent1->left != NULL) && (parent2->left != NULL)) {
		// allocate branch for children
		tree_init(&(child->left));
		EVO_crossover(child->left, parent1->left, parent2->left);
	} else {
		// flip coin
		coin = rand() % 2;

		// first parent
		if (coin) {
			tree_copy(&(child->left), &(parent1->left));
		} else {
			tree_copy(&(child->left), &(parent2->left));
		}
	}

	// up
	if ((parent1->up != NULL) && (parent2->up != NULL)) {
		// allocate branch for children
		tree_init(&(child->up));
		EVO_crossover(child->up, parent1->up, parent2->up);
	} else {
		// flip coin
		coin = rand() % 2;

		// first parent
		if (coin) {
			tree_copy(&(child->up), &(parent1->up));
		} else {
			tree_copy(&(child->up), &(parent2->up));
		}
	}

	// right
	if ((parent1->right != NULL) && (parent2->right != NULL)) {
		// allocate branch for children
		tree_init(&(child->right));
		EVO_crossover(child->right, parent1->right, parent2->right);
	} else {
		// flip coin
		coin = rand() % 2;

		// first parent
		if (coin) {
			tree_copy(&(child->right), &(parent1->right));
		} else {
			tree_copy(&(child->right), &(parent2->right));
		}
	}
}
