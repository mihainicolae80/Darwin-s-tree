

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
#define BUSY_WORKER	1
#define FREE_WORKER 0

int main(int argc, char **argv)
{
	bool run;
	char *tree_genome[EVO_UNITS_ON_GENERATION];
	treenode_t *tree[2][EVO_UNITS_ON_GENERATION];
	int i = 0, buffer = 0, generation = 0;
	//float fitness[EVO_UNITS_ON_GENERATION];
	float *fitness = malloc(sizeof(float) * EVO_UNITS_ON_GENERATION);
	float fitness_mean;
	int taskID, numTasks;

#ifdef __SDL__
	SDL_Event event;
#endif

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskID);
	MPI_Comm_size(MPI_COMM_WORLD, &numTasks);

	MPI_Status status;

	// BOSS
	if(taskID == 0) {

		char *current_tree_genome;
		char *begin_genome;
		current_tree_genome = malloc(sizeof(char) * 100);
		//remember where current genome is starting
		begin_genome = current_tree_genome;
		int workers[numTasks];
		int j = 0;
		int k = 0;
		bool allBusy;
		float current_fitness;

		// workers[0] = BOSS -> always BUSY
		workers[0] = BUSY_WORKER;
		// the rest of workers are FREE
		for(i = 1; i < numTasks; i++)
			workers[i] = FREE_WORKER;

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
			//printf("%s\n", tree_genome[i]);
			tree_init(&tree[0][i]);
			tree_build(tree[0][i], &tree_genome[i]);
		}

		// main loop
		run = true;

		for (generation = 0; generation < NUM_GEN && run; generation++) {

			printf("======= Generation %d =======\n", generation);
			fflush(stdout);

#ifdef __SDL__
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					run = false;
					break;
				}
			}
#endif
			// fitness
			fitness_mean = 0;


//SHOULD BE PARALLEL - EVO_fitness part
			for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
				if (run) {
					// extract genome from current tree 
					current_tree_genome = begin_genome;
					tree_decompose(&current_tree_genome, tree[buffer][i]);
					int len = current_tree_genome - begin_genome;
					begin_genome[len] = '\0';

					allBusy = true;
					for(j = 0; j < numTasks; j++) {
						if(workers[j] == FREE_WORKER) {
							// send new extracted genome
							MPI_Send(begin_genome, 100,
									MPI_BYTE, j, i, MPI_COMM_WORLD);
							workers[j] = BUSY_WORKER;
							allBusy = false;

							break;
						}
					}
					if(allBusy) {
						// receive fitness value for some tree in this generation
						// tag = positions of insertion in fitness array
						MPI_Recv(&current_fitness, 1, MPI_FLOAT, MPI_ANY_SOURCE,
								MPI_ANY_TAG, MPI_COMM_WORLD, &status);
						k++;
						int source = status.MPI_SOURCE;
						int index = status.MPI_TAG;
						fitness[index] = current_fitness;

						// send back new extracted genome
						MPI_Send(begin_genome, 100,
								MPI_BYTE, source, i, MPI_COMM_WORLD);

					}
				}
			}

			// receive the rest of fitness values
			// tag = positions of insertion in fitness array
			for (i = k; i < EVO_UNITS_ON_GENERATION; i++) {
				MPI_Recv(&current_fitness, 1, MPI_FLOAT, MPI_ANY_SOURCE,
						MPI_ANY_TAG, MPI_COMM_WORLD, &status);

				int index = status.MPI_TAG;
				fitness[index] = current_fitness;
			}

			k = 0;

			for(i = 1; i < numTasks; i++)
				workers[i] = FREE_WORKER;
//---------------------------------

			for(i = 0; i < EVO_UNITS_ON_GENERATION; i++)
				fitness_mean += fitness[i];

			
			// sort by fitness
			EVO_sort_by_fitness(fitness, &tree[buffer][0]);

			// crossover of fittest in other buffer
			EVO_crossover_on_generation(&tree[!buffer][0], &tree[buffer][0]);
			MISC_gen_rand();

			// mutate trees
			for (i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
				EVO_mutate(tree[!buffer][i], i);
			}

			buffer = !buffer;

			printf(	"Generation mean fitness %f\n", fitness_mean / EVO_UNITS_ON_GENERATION);
			fflush(stdout);
		}

		// all workers are done
		// tag = -1 will let them know
		for(i = 1; i < numTasks; i++) {
			MPI_Send(begin_genome, 100,
					MPI_BYTE, i, -1, MPI_COMM_WORLD);
			workers[i] = FREE_WORKER;
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
		printf("BOSS IS DONE\n");
		fflush(stdout);
	}
	// WORKERS
	else {
		char *current_tree_genome;
		current_tree_genome = malloc(sizeof(char) * 100);
		treenode_t *current_tree;
		float current_fitness;
		run = true;
		int index;

		while(run) {

			//resets
			current_tree = NULL;

			MPI_Recv(current_tree_genome, 100, MPI_BYTE,
					0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

			// worker is freeeee
			if(status.MPI_TAG == -1)
				run = false;
			// continue computing fitness values
			else {
				// save the position of current tree to send it back
				index = status.MPI_TAG;
				// rebuild the tree with new genome
				tree_init(&current_tree);
				tree_build(current_tree, &current_tree_genome);

				// compute fitness
				current_fitness = EVO_fitness(current_tree, false);
				printf("Tree[%d] from task %d fitness %f\n", index, taskID, current_fitness);
				fflush(stdout);

				MPI_Send(&current_fitness, 1, MPI_FLOAT,
						0, index, MPI_COMM_WORLD);
			}
		}



		printf("WORKER %d is DONE\n", taskID);
		fflush(stdout);
	}

	MPI_Finalize();

	return 0;
}

/**
	- paralelizare mutate;
	- modificare lungime genom cu MPI_Probe la receive sa nu mai trimit 100;
	- ca alternativa la chestia de mai sus pot sa mai fac un send cu dimensiunea;
 */
