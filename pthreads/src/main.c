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
#include "threads.h"
#include "threadpool.h"
#include <stdio.h>
#include "misc.h"

#define NUM_GEN		4


void delegate_fitness(int id) {
	fitness[id] = EVO_fitness(tree[buffer][id], false);
	printf("fitness[%d]=%f\n", id, fitness[id]);
}

void delegate_mutate(int id) {
	EVO_mutate(tree[!buffer][id], id);
}


int main(int argc, char **argv)
{

	int i, generation = 0, num_threads;
	char *aux_genome;
	float fitness_mean;
	task_t task;
#ifdef __SDL__
	SDL_Event event;
#endif
	// init
	GFX_init();
	TPOOL_init();

	if (argc > 1) {
		num_threads = atoi(argv[1]);
	} else {
		num_threads = 1;
	}

	THR_run = true;
	TPOOL_start(num_threads);

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

	// start threads
	gfx_on = false;

	// main loop
	while (THR_run) {
		// ======= EVOLVE ======
		printf("======= Generation %d ====== \n", generation);
#ifdef __SDL__
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
#endif
		// compute mean fitness
		fitness_mean = 0;
		for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
			task.callback = delegate_fitness;
			task.index = i;
			TPOOL_enqueue(task);
		}
		TPOOL_wait_for_all();
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
		MISC_gen_rand();
		for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
			task.callback = delegate_mutate;
			task.index = i;
			TPOOL_enqueue(task);
		}
		TPOOL_wait_for_all();

		buffer = !buffer;
		generation ++;

		if (NUM_GEN == generation) {
			break;
		}
	}

	TPOOL_join();

#ifdef __SDL__
	// show last best tree
	GFX_Clear(GFX_WHITE);
	TREEGFX_draw(	tree[!buffer][0],
			SCREEN_WIDTH / 2,
			SCREEN_HEIGHT - 100,
			0,
			0
	);
	GFX_Present();

	// handle events
	THR_run = true;
	while(THR_run)
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

	SDL_Quit();
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


	for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
		free(tree_genome[i]);
	}


	return 0;
}
