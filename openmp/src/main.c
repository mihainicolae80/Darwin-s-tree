#include "omp.h"
#include "graphics.h"
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

int main()
{
	//char *tree_auxgenome = "ulbrbuuulrbllbrrblbbbbrlbrrblrbl";
	//char *tree_auxgenome = "uulbrbullbrbubbrrbllbrbbu";
	//char *tree2_auxgenome = "uulbrbullbrbubbrrbllbrbbu";
	bool run;
	SDL_Event event;
	char *tree_genome[EVO_UNITS_ON_GENERATION];
	treenode_t *tree[2][EVO_UNITS_ON_GENERATION];
	int i = 0, buffer = 0, generation = 0;
	float fitness[EVO_UNITS_ON_GENERATION], fitness_mean;
	FILE *fitness_graph_file;

	// init
	GFX_init();
	srand(time(NULL));

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

	// main loop
	run = true;
	#pragma omp parallel private(generation, buffer)
	{	
		for (generation = 0; generation <= 5 && run; generation++) {

			#pragma omp single
			{
				// ======= EVOLVE ======
				printf("======= Generation %d\n", generation);
			}
			
			// fitness
			fitness_mean = 0;

			#pragma omp for private(i, fitness) reduction(+:fitness_mean)
			for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
					if (run) {
						fitness[i] = EVO_fitness(tree[buffer][i], false);
						printf("tree[%d] fitness %f\n", i, fitness[i]);
						fitness_mean += fitness[i];
					}
			}

			// handle events
			#pragma omp master
			{
				while (SDL_PollEvent(&event)) {

					if (event.type == SDL_QUIT) {
						run = false;
						break;
					}
				}
			}

			// sort by fitness
			EVO_sort_by_fitness(fitness, &tree[buffer][0]);

			// crossover of fittest in other buffer
			EVO_crossover_on_generation(&tree[!buffer][0], &tree[buffer][0]);

			// mutate trees
			#pragma omp parallel private(i)
			for (i = 0; i < EVO_UNITS_ON_GENERATION; i++)
				EVO_mutate(tree[!buffer][i]);

			#pragma omp single
			{
				printf(	"Generation mean fitness %f\n", fitness_mean / EVO_UNITS_ON_GENERATION);
				fprintf(fitness_graph_file, "%d ", (int)(fitness_mean / EVO_UNITS_ON_GENERATION));
				buffer = !buffer;
			}
		}
	}

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
