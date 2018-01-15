#include "graphics.h"
#include "conf_graphics.h"
#include "conf_tree_gfx.h"
#include "tree.h"
#include "tree_gfx.h"
#include <stdlib.h>
#include <time.h>
#include "evolution.h"
#include "conf_evolution.h"
#include "misc.h"

#define NUM_GEN		4


int main()
{
	bool run, gfx_on;
	char *tree_genome[EVO_UNITS_ON_GENERATION], *aux_genome;
	treenode_t *tree[2][EVO_UNITS_ON_GENERATION];
	int i, buffer = 0, generation = 0, gen;
	float fitness[EVO_UNITS_ON_GENERATION], fitness_mean;
#ifdef __SDL__
	SDL_Event event;
#endif
	// init
	GFX_init();
	srand(0);
	for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
		tree[0][i] = NULL;
		tree[1][i] = NULL;
	}

	// generate initial population
	for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
		tree_genome[i] = malloc(sizeof(char) * EVO_INITIAL_NUM_BRANCHES);
		EVO_get_random_genome(tree_genome[i], EVO_INITIAL_NUM_BRANCHES);
		tree_init(&tree[0][i]);
		aux_genome = tree_genome[i];
		tree_build(tree[0][i], &aux_genome);
	}

	// main loop
	run = true;
	gfx_on = true;

	for (gen = 0; gen < NUM_GEN; gen ++){

		// ======= EVOLVE ======
		printf("======= Generation %d ====== \n", generation);

		// fitness
		fitness_mean = 0;
		for (i = 0; i < EVO_UNITS_ON_GENERATION && run; i++) {
			fitness[i] = EVO_fitness(tree[buffer][i], gfx_on);
			printf("tree[%d] fitness %f\n",i, fitness[i]);
			fitness_mean += fitness[i];
#ifdef __SDL__
			// handle events
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					run = false;
					break;
				} else if (event.type == SDL_KEYDOWN) {
					if (event.key.keysym.sym == SDLK_g)
						gfx_on = !gfx_on;
					else if (event.key.keysym.sym == SDLK_s)  {
						run = false;
						break;
					}
				}
			}
#endif
		}

		// sort by fitness
		EVO_sort_by_fitness(fitness, &tree[buffer][0]);
		// crossover of fittest in other buffer
		EVO_crossover_on_generation(&tree[!buffer][0], &tree[buffer][0]);
		// mutate trees
		MISC_gen_rand();
		for (i = 0; i < EVO_UNITS_ON_GENERATION; i++)
			EVO_mutate(tree[!buffer][i], i);

		printf(	"Generation mean fitness %f\n",
			fitness_mean / EVO_UNITS_ON_GENERATION
		);

		buffer = !buffer;
		generation ++;
	}

	// show last best tree
	GFX_Clear(GFX_WHITE);
	TREEGFX_draw(	tree[!buffer][0],
			SCREEN_WIDTH / 2,
			SCREEN_HEIGHT - 100,
			0,
			0
	);
	GFX_Present();

#ifdef __SDL__
	// handle events
	run = true;
	while(run)
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			run = false;
			break;
		} else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_g)
				gfx_on = !gfx_on;
			else if (event.key.keysym.sym == SDLK_s)  {
				run = false;
				break;
			}
		}
	}
#endif

	for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
		if (tree[0][i] != NULL) {
			tree_free(&tree[0][i]);
		}
		if (tree[1][i] != NULL) {
			tree_free(&tree[1][i]);
		}
	}

	//free(tree_genome);
#ifdef __SDL__
	SDL_Quit();
#endif

	for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
		free(tree_genome[i]);
	}

	return 0;
}
