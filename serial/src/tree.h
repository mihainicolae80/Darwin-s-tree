#ifndef  TREE__H_
#define TREE__H_ value

#include "SDL2/SDL.h"

typedef struct tree {
	struct tree *left, *right, *up;
} treenode_t;

typedef struct leafarray{
	SDL_Rect *lears;
	int size;
}leafarray_t;


void tree_init(treenode_t **root);
void tree_copy(treenode_t **dest, treenode_t **source);
void tree_build(treenode_t *node, char **branches);
void tree_free(treenode_t **root);
void tree_iterate(treenode_t *root);
void tree_get_size(treenode_t *root);
void tree_copy_leafs(treenode_t *root);
void tree_get_leafs(treenode_t *node, SDL_Rect *leafs, int *index, int x, int y, int angle, int depth);

#endif
