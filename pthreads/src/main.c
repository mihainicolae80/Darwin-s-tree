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
#include <pthread.h>
#include "threads.h"
#include <unistd.h>

int main()
{
	bool gfx_on;
	SDL_Event event;
	int i, generation = 0;
	FILE *fitness_graph_file;
	FILE *time_graph_file;
	char *aux_genome;
	float fitness_mean;
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

	// prepare locks
	pthread_barrier_init(&THR_barrier_1, NULL, EVO_UNITS_ON_GENERATION + 1);
	pthread_barrier_init(&THR_barrier_2, NULL, EVO_UNITS_ON_GENERATION + 1);
	pthread_barrier_init(&THR_barrier_3, NULL, EVO_UNITS_ON_GENERATION + 1);

	// start threads
	THR_run = true;
	gfx_on = false;
	start_time = SDL_GetTicks();
	THR_start_threads();
	// main loop
	while (THR_run) {
		// ======= EVOLVE ======
		printf("======= Generation %d ====== \n", generation);
		// wait for fitness data
		pthread_barrier_wait(&THR_barrier_1);
		// handle events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				THR_run = false;
				break;
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_g)
					gfx_on = !gfx_on;
				else if (event.key.keysym.sym == SDLK_s)  {
					THR_run = false;
					break;
				}
			}
		}

		// compute mean fitness
		fitness_mean = 0;
		for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
			fitness_mean += fitness[i];
		}
		fitness_mean /= EVO_UNITS_ON_GENERATION;
		// sort by fitness
		EVO_sort_by_fitness(fitness, &tree[buffer][0]);
		// crossover of fittest in other buffer
		EVO_crossover_on_generation(&tree[!buffer][0], &tree[buffer][0]);
		printf(	"Generation mean fitness %f\n",
			fitness_mean
		);
		fprintf(fitness_graph_file, "%d\n", (int)(fitness_mean));
		fprintf(time_graph_file, "%d\n", (int)(SDL_GetTicks() - start_time));
		pthread_barrier_wait(&THR_barrier_2);
		buffer = !buffer;
		generation ++;
		pthread_barrier_wait(&THR_barrier_3);
	}
	THR_wait_for_threads();


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
	THR_run = true;
	while (THR_run) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				THR_run = false;
				break;
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_s)  {
					THR_run = false;
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
