#include "evolution.h"
#include "conf_evolution.h"
#include "misc.h"
#include "graphics.h"
#include "conf_graphics.h"
#include "tree.h"
#include "tree_gfx.h"
#include "conf_tree_gfx.h"


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

void EVO_mutate(treenode_t *tree, int unit_index)
{
	// left
	if (NULL == tree->left) {
		if (rand_val[unit_index][rand_index[unit_index]++] % EVO_MUTATION_ADD_ONEIN == 0)
			tree_init(&(tree->left));
	} else if ((NULL == tree->left->left)
		&& (NULL == tree->left->up)
		&& (NULL == tree->left->right)) {
		if (rand_val[unit_index][rand_index[unit_index]++] % EVO_MUTATION_RM_ONEIN == 0) {
			tree_free(&(tree->left));
		}
	}
	if (tree->left)
		EVO_mutate(tree->left, unit_index);


	// right
	if (NULL == tree->right) {
		if (rand_val[unit_index][rand_index[unit_index]++] % EVO_MUTATION_ADD_ONEIN == 0)
			tree_init(&(tree->right));
	} else if ((NULL == tree->right->left)
		&& (NULL == tree->right->up)
		&& (NULL == tree->right->right)) {
		if (rand_val[unit_index][rand_index[unit_index]++] % EVO_MUTATION_RM_ONEIN == 0) {
			tree_free(&(tree->right));
		}
	}
	if (tree->right)
		EVO_mutate(tree->right, unit_index);

	// up
	if (NULL == tree->up) {
		if (rand_val[unit_index][rand_index[unit_index]++] % EVO_MUTATION_ADD_ONEIN == 0)
			tree_init(&(tree->up));
	} else if ((NULL == tree->up->left)
		&& (NULL == tree->up->up)
		&& (NULL == tree->up->right)) {
		if (rand_val[unit_index][rand_index[unit_index]++] % EVO_MUTATION_RM_ONEIN == 0) {
			tree_free(&(tree->up));
		}
	}
	if (tree->up)
		EVO_mutate(tree->up, unit_index);
}

#ifdef __SDL__

float EVO_fitness(treenode_t *tree, bool render)
{
	int sun_x = 0, sun_y = 0, subleafs_lit = 0, lx, ly, k, q, l, index = 0, i;
	int num_branches = 0, maxdepth;
	rect_t subleaf;
	float px, py;
	bool collision;
	rect_t leafs[TREEGFX_MAX_NUM_LEAF];
	const int dx[] = {	-EVO_LEAF_SIZE/3,	0, EVO_LEAF_SIZE/3,
				-EVO_LEAF_SIZE/3, 	0, EVO_LEAF_SIZE/3,
				-EVO_LEAF_SIZE/3,	0, EVO_LEAF_SIZE/3};
	const int dy[] = {	-EVO_LEAF_SIZE/3, -EVO_LEAF_SIZE/3,-EVO_LEAF_SIZE/3,
					0,		0,		0,
			 	EVO_LEAF_SIZE/3,  EVO_LEAF_SIZE/3, EVO_LEAF_SIZE/3};

	maxdepth = 0;
	// get all leafs as rectangles
	tree_get_leafs(	tree, leafs, &index, &num_branches,
			SCREEN_WIDTH / 2,
			SCREEN_HEIGHT - 100,
			0,
			0,
			&maxdepth
	);

	for (sun_x = 0; sun_x < SCREEN_WIDTH; sun_x += EVO_SUN_STEP_INC) {
		if (render) {
			GFX_Clear(GFX_WHITE);
			TREEGFX_draw(	tree,
					SCREEN_WIDTH / 2,
					SCREEN_HEIGHT - 100,
					0,
					0
			);
		}

		// light on every leaf
		sun_y = TREEGFX_EARTH_LEVEL - (TREEGFX_EARTH_LEVEL - 50)
			* sin((float)PI * sun_x / SCREEN_WIDTH);
		for (i = 0; i < index; i++) {
			// leaf on every subleaf
			for (q = 0; q < 9; q ++) {
				// subleaf center coordinates
				lx = leafs[i].x + leafs[i].w/2 + dx[q];
				ly = leafs[i].y + leafs[i].h/2 + dy[q];
				// compute intermediary points
				collision = false;
				for(l = 0; l < EVO_SUN_SAMPLES && !collision; l++) {
					px = lx + l *((float)sun_x - lx) / EVO_SUN_SAMPLES;
					py = ly + l * ((float)sun_y - ly) / EVO_SUN_SAMPLES;

					// check collision with all other blocks
					for (k = 0; k < index; k++) {
						if ((k != i)
						&& (px >= leafs[k].x)
						&& (px <= leafs[k].x + leafs[k].w)
						&& (py >= leafs[k].y)
						&& (py <= leafs[k].y + leafs[k].h)
						){
							collision = true;
							break;
						}
					}
				}

				if (!collision) {
					subleafs_lit ++;
					subleaf.x = lx - EVO_LEAF_SIZE / 6;
					subleaf.y = ly - EVO_LEAF_SIZE / 6;
					subleaf.w = EVO_LEAF_SIZE / 3;
					subleaf.h = EVO_LEAF_SIZE / 3;


					if (render) {
						SDL_SetRenderDrawColor(	_render,
							GFX_COLOR_GET_RED(GFX_GREEN),
							GFX_COLOR_GET_GREEN(GFX_GREEN),
							GFX_COLOR_GET_BLUE(GFX_GREEN),
							GFX_COLOR_GET_ALPHA(GFX_GREEN)
						);
						SDL_Rect aux;
						aux.x = subleaf.x;
						aux.y = subleaf.y;
						aux.w = subleaf.w;
						aux.h = subleaf.h;
						SDL_RenderFillRect(_render, &aux);
					}
				} else {
					if (render) {
						SDL_SetRenderDrawColor(	_render,
							GFX_COLOR_GET_RED(GFX_RED),
							GFX_COLOR_GET_GREEN(GFX_RED),
							GFX_COLOR_GET_BLUE(GFX_RED),
							GFX_COLOR_GET_ALPHA(GFX_RED)
						);
					}
				}

				if (render) {
					SDL_RenderDrawLine(_render, lx, ly, sun_x, sun_y);
				}
			}

		}
		if (render) {
			GFX_Present();
			//SDL_Delay(10);
		}
	}

	return (float)subleafs_lit * pow(0.90, maxdepth);
}
#else

float EVO_fitness(treenode_t *tree, bool render)
{
	int sun_x = 0, sun_y = 0, subleafs_lit = 0, lx, ly, k, q, l, index = 0, i;
	int num_branches = 0, maxdepth;
	float px, py;
	bool collision;
	rect_t leafs[TREEGFX_MAX_NUM_LEAF];
	const int dx[] = {	-EVO_LEAF_SIZE/3,	0, EVO_LEAF_SIZE/3,
				-EVO_LEAF_SIZE/3, 	0, EVO_LEAF_SIZE/3,
				-EVO_LEAF_SIZE/3,	0, EVO_LEAF_SIZE/3};
	const int dy[] = {	-EVO_LEAF_SIZE/3, -EVO_LEAF_SIZE/3,-EVO_LEAF_SIZE/3,
					0,		0,		0,
			 	EVO_LEAF_SIZE/3,  EVO_LEAF_SIZE/3, EVO_LEAF_SIZE/3};

	maxdepth = 0;
	// get all leafs as rectangles
	tree_get_leafs(	tree, leafs, &index, &num_branches,
			SCREEN_WIDTH / 2,
			SCREEN_HEIGHT - 100,
			0,
			0,
			&maxdepth
	);

	for (sun_x = 0; sun_x < SCREEN_WIDTH; sun_x += EVO_SUN_STEP_INC) {

		// light on every leaf
		sun_y = TREEGFX_EARTH_LEVEL - (TREEGFX_EARTH_LEVEL - 50)
			* sin((float)PI * sun_x / SCREEN_WIDTH);
		for (i = 0; i < index; i++) {
			// leaf on every subleaf
			for (q = 0; q < 9; q ++) {
				// subleaf center coordinates
				lx = leafs[i].x + leafs[i].w/2 + dx[q];
				ly = leafs[i].y + leafs[i].h/2 + dy[q];
				// compute intermediary points
				collision = false;
				for(l = 0; l < EVO_SUN_SAMPLES && !collision; l++) {
					px = lx + l *((float)sun_x - lx) / EVO_SUN_SAMPLES;
					py = ly + l * ((float)sun_y - ly) / EVO_SUN_SAMPLES;

					// check collision with all other blocks
					for (k = 0; k < index; k++) {
						if ((k != i)
						&& (px >= leafs[k].x)
						&& (px <= leafs[k].x + leafs[k].w)
						&& (py >= leafs[k].y)
						&& (py <= leafs[k].y + leafs[k].h)
						){
							collision = true;
							break;
						}
					}
				}

				if (!collision) {
					subleafs_lit ++;
				} else {
				}
			}

		}
	}

	return (float)subleafs_lit * pow(0.90, maxdepth);
}

#endif


void EVO_get_random_genome(char genome[], int maxlen)
{
	int i, c, depth;

	genome[0] = 'u';
	depth = 1;
	for (i = 1; i < maxlen-1; i++) {
		c = rand() % 4;
		genome[i] = 	(c == 0) ? 'u' :
				(c == 1) ? 'l' :
				(c == 2) ? 'r' :
					   'b';

		depth += (c == 3) ? -1 : 1;
		if (0 == depth) {
			genome[i] = '\0';
			break;
		}
	}

	genome[i] = '\0';
}

void EVO_draw_tree(treenode_t *tree)
{
	GFX_Clear(GFX_WHITE);

	TREEGFX_draw(	tree,
			SCREEN_WIDTH / 2,
			SCREEN_HEIGHT - 100,
			0,
			0
	);

	TREEGFX_draw_earth();
	GFX_Present();
}

void EVO_sort_by_fitness(float *fitness, treenode_t **trees)
{
	int i;
	float aux_fit;
	treenode_t *aux_tree;
	bool change;

	do {
		change = false;
		for (i = 0; i < EVO_UNITS_ON_GENERATION -1; i++) {
			if (fitness[i + 1] > fitness[i]) {
				aux_fit = fitness[i + 1];
				fitness[i + 1] = fitness[i];
				fitness[i] = aux_fit;

				aux_tree = trees[i + 1];
				trees[i + 1] = trees[i];
				trees[i] = aux_tree;

				change = true;
			}
		}
	}while(change);
}

void 	EVO_crossover_on_generation(treenode_t **dest, treenode_t **src)
{
	/* The second half of the trees is discarded (worst performers)
	   Each tree does a crossover with all the less performing trees
	*/
	int i, j, k;

	k = 0;
	for (i = 0; i < EVO_UNITS_ON_GENERATION / 2; i++) {
		for (j = i + 1; j < EVO_UNITS_ON_GENERATION / 2; j++) {
			// prepare tree
			if (dest[k] != NULL) {
				tree_free(&dest[k]);
			}
			tree_init(&dest[k]);
			// crossover
			EVO_crossover(dest[k++], src[i], src[j]);
		}
	}
}
