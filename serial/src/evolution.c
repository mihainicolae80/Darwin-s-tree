#include "evolution.h"
#include "conf_evolution.h"


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

void EVO_mutate(treenode_t *tree)
{
	// left
	if (NULL == tree->left) {
		if (rand() % EVO_MUTATION_ADD_ONEIN == 0)
			tree_init(&(tree->left));
	} else if ((NULL == tree->left->left)
		&& (NULL == tree->left->up)
		&& (NULL == tree->left->right)) {
		if (rand() % EVO_MUTATION_RM_ONEIN == 0) {
			tree_free(&(tree->left));
		}
	}
	if (tree->left)
		EVO_mutate(tree->left);


	// right
	if (NULL == tree->right) {
		if (rand() % EVO_MUTATION_ADD_ONEIN == 0)
			tree_init(&(tree->right));
	} else if ((NULL == tree->right->left)
		&& (NULL == tree->right->up)
		&& (NULL == tree->right->right)) {
		if (rand() % EVO_MUTATION_RM_ONEIN == 0) {
			tree_free(&(tree->right));
		}
	}
	if (tree->right)
		EVO_mutate(tree->right);

	// up
	if (NULL == tree->up) {
		if (rand() % EVO_MUTATION_ADD_ONEIN == 0)
			tree_init(&(tree->up));
	} else if ((NULL == tree->up->left)
		&& (NULL == tree->up->up)
		&& (NULL == tree->up->right)) {
		if (rand() % EVO_MUTATION_RM_ONEIN == 0) {
			tree_free(&(tree->up));
		}
	}
	if (tree->up)
		EVO_mutate(tree->up);
}
