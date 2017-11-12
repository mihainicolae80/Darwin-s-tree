#ifndef  TREE__H_
#define TREE__H_ value


typedef struct tree {
	struct tree *left, *right, *up;
} treenode_t;


void tree_init(treenode_t **root);
void tree_copy(treenode_t **dest, treenode_t **source);
void tree_build(treenode_t *node, char **branches);
void tree_free(treenode_t **root);
void tree_iterate(treenode_t *root);

#endif
