#include <mpi.h>
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
	int i = 0, buffer = 0, generation = 0;
	//float fitness[EVO_UNITS_ON_GENERATION];
	float *fitness = malloc(sizeof(float) * EVO_UNITS_ON_GENERATION);
	float fitness_mean, local_fitness_mean;
	int taskID, numTasks;
	int chunk, start, end;

#ifdef __SDL__
	SDL_Event event;
#endif

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskID);
	MPI_Comm_size(MPI_COMM_WORLD, &numTasks);

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

	
	// share the work between threads
	chunk = EVO_UNITS_ON_GENERATION / numTasks;
	start = chunk * taskID;
	end = (taskID == numTasks - 1) ? start + (EVO_UNITS_ON_GENERATION - (numTasks - 1) * chunk) : 
									 start + chunk;
	MPI_Barrier(MPI_COMM_WORLD);
	// main loop --------------------------------------------------------------------
	run = true;

	for (generation = 0; generation < NUM_GEN && run; generation++) {

		if(taskID == 0) {
			printf("======= Generation %d\n", generation);
		}

#ifdef __SDL__		
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				run = false;
				break;
			}
		}
#endif
		// fitness
		local_fitness_mean = 0;
		fitness_mean = 0;


//SHOULD BE PARALLEL - EVO_fitness
// fiecare thread calculeaza o suma locala, apoi se face reduce
		
		for (i = start; i < end; i++) {
			if (run) {
				fitness[i] = EVO_fitness(tree[buffer][i], false);
				printf("tree[%d] fitness %f\n", i, fitness[i]);
				local_fitness_mean += fitness[i];
			}
		}
		//printf("Local sum on thread %d is %f\n", taskID, local_fitness_mean);
		MPI_Reduce(&local_fitness_mean, &fitness_mean, 1, MPI_FLOAT, MPI_SUM, 0,
           MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
//---------------------------------

		if(taskID == 0) {
			printf(	"Generation mean fitness %f\n", fitness_mean / EVO_UNITS_ON_GENERATION);	
		}

		// sort by fitness
		EVO_sort_by_fitness(fitness, &tree[buffer][0]);

		// crossover of fittest in other buffer
		EVO_crossover_on_generation(&tree[!buffer][0], &tree[buffer][0]);
		MISC_gen_rand();

		MPI_Barrier(MPI_COMM_WORLD);

//SHOULD BE PARALLEL - EVO_mutate
		// mutate trees
		for (i = start; i < end; i++)
			EVO_mutate(tree[!buffer][i], i);
		MPI_Barrier(MPI_COMM_WORLD);
//-------------------

		buffer = !buffer;
	}

	MPI_Finalize(); //-----------------------------------------------------------------

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