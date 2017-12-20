#include "omp.h"
#include "conf_graphics.h"
#include "conf_tree_gfx.h"
#include "tree.h"
#include "tree_gfx.h"
#include <stdlib.h>
#include <time.h>
#include "evolution.h"
#include "conf_evolution.h"
#include "misc.h"
#include <stdint.h>

#define NUM_THREADS	6
#define NUM_GEN		4

int main(int argc, char **argv)
{
	bool run;
	char *tree_genome[EVO_UNITS_ON_GENERATION];
	treenode_t *tree[2][EVO_UNITS_ON_GENERATION];
	int i = 0, buffer = 0, generation = 0, num_threads;
	//float fitness[EVO_UNITS_ON_GENERATION];
	float *fitness = malloc(sizeof(float) * EVO_UNITS_ON_GENERATION);
	float fitness_mean;

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
		tree_build(tree[0][i], &tree_genome[i]);
	}

	if (argc > 1) {
		num_threads = atoi(argv[1]);
	} else {
		num_threads = 4;
	}

	omp_set_num_threads(num_threads);

	// main loop
	run = true;

	#pragma omp parallel firstprivate(generation)
	{
		for (generation = 0; generation < NUM_GEN && run; generation++) {

#ifdef __SDL__
			#pragma omp single
			{
				// ======= EVOLVE ======
				printf("======= Generation %d\n", generation);
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_QUIT) {
						run = false;
						break;
					}
				}
			}
#endif
			// fitness
			fitness_mean = 0;

			#pragma omp for private(i) reduction(+:fitness_mean)
			for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
				if (run) {
					fitness[i] = EVO_fitness(tree[buffer][i], false);
					fitness_mean += fitness[i];
				}
			}

			#pragma omp single
			{
				for (i = 0; i < EVO_UNITS_ON_GENERATION; i++)
					printf("tree[%d] fitness %f\n", i, fitness[i]);

				// sort by fitness
				EVO_sort_by_fitness(fitness, &tree[buffer][0]);

				// crossover of fittest in other buffer
				EVO_crossover_on_generation(&tree[!buffer][0], &tree[buffer][0]);
				MISC_gen_rand();
			}

			// mutate trees
			#pragma omp for private(i)
			for (i = 0; i < EVO_UNITS_ON_GENERATION; i++)
				EVO_mutate(tree[!buffer][i], i);

			#pragma omp single
			{
				printf(	"Generation mean fitness %f\n", fitness_mean / EVO_UNITS_ON_GENERATION);
				buffer = !buffer;
			}
		}
	}

#ifdef __SDL__
	GFX_Clear(GFX_WHITE);
	TREEGFX_draw(	tree[!buffer][0],
			SCREEN_WIDTH / 2,
			SCREEN_HEIGHT - 100,
			0,
			0
	);
	GFX_Present();

	run = true;
	while (run)
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			run = false;
			break;
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

	return 0;
}
