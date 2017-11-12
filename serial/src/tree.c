#include "tree.h"
#include <malloc.h>
#include <stdio.h>

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
