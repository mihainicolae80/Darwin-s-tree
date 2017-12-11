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
	bool run, gfx_on;
	SDL_Event event;
	char *tree_genome[EVO_UNITS_ON_GENERATION];
	treenode_t *tree[2][EVO_UNITS_ON_GENERATION];
	int i, buffer = 0, generation = 0;
	float fitness[EVO_UNITS_ON_GENERATION], fitness_mean;
	FILE *fitness_graph_file;
	FILE *time_graph_file;
	char *aux_genome;
	uint32_t start_time;


	// init
	GFX_init();
	srand(0);
	for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
		tree[0][i] = NULL;
		tree[1][i] = NULL;
	}
	fitness_graph_file = fopen("fitness.out", "w");
	time_graph_file = fopen("time.out", "w");

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
	gfx_on = false;
	start_time = SDL_GetTicks();
	while (run) {
		// ======= EVOLVE ======
		printf("======= Generation %d ====== \n", generation);

		// fitness
		fitness_mean = 0;
		for (i = 0; i < EVO_UNITS_ON_GENERATION && run; i++) {
			fitness[i] = EVO_fitness(tree[buffer][i], gfx_on);
			printf("tree[%d] fitness %f\n",i, fitness[i]);
			fitness_mean += fitness[i];

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
		}

		// sort by fitness
		EVO_sort_by_fitness(fitness, &tree[buffer][0]);
		// crossover of fittest in other buffer
		EVO_crossover_on_generation(&tree[!buffer][0], &tree[buffer][0]);
		// mutate trees
		for (i = 0; i < EVO_UNITS_ON_GENERATION; i++)
			EVO_mutate(tree[!buffer][i]);

		printf(	"Generation mean fitness %f\n",
			fitness_mean / EVO_UNITS_ON_GENERATION
		);

		fprintf(fitness_graph_file, "%d\n", (int)(fitness_mean / EVO_UNITS_ON_GENERATION));
		fprintf(time_graph_file, "%d\n", (int)(SDL_GetTicks() - start_time));

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

	// wait for key press
	run = true;
	while (run) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				run = false;
				break;
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_s)  {
					run = false;
					break;
				}
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
	fclose(time_graph_file);
	for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
		free(tree_genome[i]);
	}

	return 0;
}
