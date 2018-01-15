#ifndef  TREE__H_
#define TREE__H_ value

#include "misc.h"

#ifdef __SDL__
#include "SDL2/SDL.h"
#endif

typedef struct tree {
	struct tree *left, *right, *up;
} treenode_t;

typedef struct leafarray{
	rect_t *lears;
	int size;
}leafarray_t;


void tree_init(treenode_t **root);
void tree_copy(treenode_t **dest, treenode_t **source);
void tree_build(treenode_t *node, char **branches);
void tree_free(treenode_t **root);
void tree_iterate(treenode_t *root);
void tree_get_size(treenode_t *root);
void tree_copy_leafs(treenode_t *root);
void tree_get_leafs(treenode_t *node, rect_t *leafs, int *index, int *num_branch, int x, int y, int angle, int depth, int *maxdepth);
void tree_decompose(char** genome, treenode_t *root);

#endif
