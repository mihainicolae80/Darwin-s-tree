#include "omp.h"
#include "conf_graphics.h"
#include "conf_tree_gfx.h"
#include "tree.h"
#include "tree_gfx.h"
#include <stdlib.h>
#include <time.h>
#include "evolution.h"
#include "sun.h"
#include "conf_evolution.h"
#include "misc.h"
#include <stdint.h>

#define NUM_THREADS	6
#define NUM_GEN		4

int main()
{
	bool run;
	SDL_Event event;
	char *tree_genome[EVO_UNITS_ON_GENERATION];
	treenode_t *tree[2][EVO_UNITS_ON_GENERATION];
	int i = 0, buffer = 0, generation = 0;
	//float fitness[EVO_UNITS_ON_GENERATION];
	float *fitness = malloc(sizeof(float) * EVO_UNITS_ON_GENERATION);
	float fitness_mean;
	FILE *fitness_graph_file, *time_cores_file;
	char filename[100];
	uint64_t time_start;

	// init
	GFX_init();
	srand(0);

	for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
		tree[0][i] = NULL;
		tree[1][i] = NULL;
	}
	fitness_graph_file = fopen("fitness.out", "w");


	// generate initial population
	for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
		tree_genome[i] = malloc(sizeof(char) * EVO_INITIAL_NUM_BRANCHES);
		EVO_get_random_genome(tree_genome[i], EVO_INITIAL_NUM_BRANCHES);
		tree_init(&tree[0][i]);
		tree_build(tree[0][i], &tree_genome[i]);
	}

	omp_set_num_threads(NUM_THREADS);

	// main loop
	run = true;
	time_start = SDL_GetTicks();
	sprintf(filename, "time_cores_%d.dat", NUM_THREADS);
	time_cores_file = fopen(filename, "w");

	#pragma omp parallel firstprivate(generation)
	{
		for (generation = 0; generation < NUM_GEN && run; generation++) {

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

			// fitness
			fitness_mean = 0;

			#pragma omp for private(i) reduction(+:fitness_mean)
			for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
				if (run) {
					fitness[i] = EVO_fitness(tree[buffer][i], false);
					//printf("tree[%d] fitness %f\n", i, fitness[i]);
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
				fprintf(fitness_graph_file, "%d ", (int)(fitness_mean / EVO_UNITS_ON_GENERATION));
				buffer = !buffer;
			}
		}
	}

	fprintf(time_cores_file, "%lu", SDL_GetTicks() - time_start);
	fclose(time_cores_file);

	for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
		if (tree[0][i] != NULL) {
			tree_free(&tree[0][i]);
		}
		if (tree[1][i] != NULL) {
			tree_free(&tree[1][i]);
		}
	}

	//free(tree_genome);
	SDL_Quit();
	fclose(fitness_graph_file);

	return 0;
}
