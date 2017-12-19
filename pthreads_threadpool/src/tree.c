#include "tree.h"
#include <malloc.h>
#include <stdio.h>
#include "misc.h"
#include "conf_tree_gfx.h"
#include "conf_evolution.h"

void tree_init(treenode_t **root)
{
	*root = malloc(sizeof(treenode_t));
	(*root)->left 	=
	(*root)->right 	=
	(*root)->up 	= NULL;
}

void tree_build(treenode_t *node, char **branches)
{

	if (**branches == '\0') {
		return;
	}

	while (**branches != '\0') {
		switch (**branches) {
		case 'l':
			if (NULL == node->left)
				tree_init(&(node->left));
			*branches += 1;
			tree_build(node->left, branches);
		break;
		case 'r':
			if (NULL == node->right)
				tree_init(&(node->right));
			*branches += 1;
			tree_build(node->right, branches);
		break;
		case 'u':
			if (NULL == node->up)
				tree_init(&(node->up));
			*branches += 1;
			tree_build(node->up, branches);
		default:
			*branches += 1;
			return;
		}
	}
}

// TODO Check
void tree_copy(treenode_t **dest, treenode_t **source)
{
	if (*source == NULL) {
		*dest = NULL;
		return;
	}

	tree_init(dest);
	tree_copy(&((*dest)->left), &((*source)->left));
	tree_copy(&((*dest)->up), &((*source)->up));
	tree_copy(&((*dest)->right), &((*source)->right));
}

void tree_free(treenode_t **root)
{
	if (*root != NULL) {
		// Recursively free branches
		if ((*root)->left)
			tree_free(&((*root)->left));
		if ((*root)->right)
			tree_free(&((*root)->right));
		if ((*root)->up)
			tree_free(&((*root)->up));
		// Free node
		free(*root);
		*root = NULL;
	}
}

void tree_iterate(treenode_t *root)
{
	if (root) {
		if (root->left) {
			printf("l\n");
			tree_iterate(root->left);
		}

		if (root->up) {
			printf("u\n");
			tree_iterate(root->up);
		}

		if (root->right) {
			printf("r\n");
			tree_iterate(root->right);
		}

		printf("b\n");
	}
}


void tree_get_leafs(	treenode_t *node, SDL_Rect *leafs, int *index,
			int *num_branch, int x, int y, int angle, int depth, int *maxdepth)
{
	float delta_x, delta_y;
	float decay;
	if (node != NULL) {
		if (depth > *maxdepth)
			*maxdepth = depth;

		(*num_branch) ++;
		// add new leaf to array
		if (TREEGFX_MAX_NUM_LEAF <= (*index)) {
			printf("Max nr of leafs exceeded!\n");
			return;
		} else if (!node->left && !node->right && !node->up){
			leafs[(*index)].x = x - EVO_LEAF_SIZE/2;
			leafs[(*index)].y = y - EVO_LEAF_SIZE/2;
			leafs[(*index)].w = EVO_LEAF_SIZE;
			leafs[(*index)++].h = EVO_LEAF_SIZE;
		}

		decay = pow(TREEGFX_BRANCH_DECAY, depth);
		// left
		delta_x = TREEGFX_BRANCH_LEN
			* sin(TO_RADIANS(angle - TREEGFX_BRANCH_ANGLE));
		delta_y = -TREEGFX_BRANCH_LEN
			* cos(TO_RADIANS(angle - TREEGFX_BRANCH_ANGLE));
		delta_x *= decay;
		delta_y *= decay;
		tree_get_leafs(	node->left, leafs, index, num_branch,
				delta_x + x,
				delta_y + y,
				BOUND_ANGLE(angle - TREEGFX_BRANCH_ANGLE),
				depth + 1,
				maxdepth
		);
		/// up
		delta_x = TREEGFX_BRANCH_LEN * sin(TO_RADIANS(angle));
		delta_y = -TREEGFX_BRANCH_LEN * cos(TO_RADIANS(angle));
		delta_x *= decay;
		delta_y *= decay;
		tree_get_leafs(	node->up, leafs, index, num_branch,
				delta_x + x,
				delta_y + y,
				angle,
				depth + 1,
				maxdepth
		);
		// right
		delta_x = TREEGFX_BRANCH_LEN
			* sin(TO_RADIANS(angle + TREEGFX_BRANCH_ANGLE));
		delta_y = -TREEGFX_BRANCH_LEN
			* cos(TO_RADIANS(angle + TREEGFX_BRANCH_ANGLE));
		delta_x *= decay;
		delta_y *= decay;
		tree_get_leafs(	node->right, leafs, index, num_branch,
				delta_x + x,
				delta_y + y,
				BOUND_ANGLE(angle + TREEGFX_BRANCH_ANGLE),
				depth + 1,
				maxdepth
		);
	}
}
