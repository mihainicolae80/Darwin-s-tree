#include "threads.h"
#include <stdio.h>
#include <stdint.h>
#include "evolution.h"

static pthread_t _threads[20];
bool THR_run;


char *tree_genome[EVO_UNITS_ON_GENERATION];
treenode_t *tree[2][EVO_UNITS_ON_GENERATION];
int  buffer = 0;
float fitness[EVO_UNITS_ON_GENERATION];
pthread_mutex_t THR_canwork;
pthread_cond_t THR_canwork_cond;
pthread_mutex_t THR_done[EVO_UNITS_ON_GENERATION];
pthread_cond_t THR_done_cond[EVO_UNITS_ON_GENERATION];
pthread_barrier_t THR_barrier_1, THR_barrier_2, THR_barrier_3;
bool gfx_on;



void THR_start_threads(int num_threads, thread_foo_t foo)
{
	if (foo) {
		for (int64_t i = 0; i < num_threads; i++) {
			if (pthread_create(&_threads[i], NULL, foo, (void*)i)) {
				printf("Error creating thread[%lu]\n", i);
			}
		}
	}
}

void THR_wait_for_threads(void)
{
	void *status;

	for(uint64_t i = 0; i < EVO_UNITS_ON_GENERATION; i++)
		if (pthread_join(_threads[i], &status)) {
			printf("Error exiting thread[%lu]\n", i);
		}
}

// DEPRECATED
void *THR_thread(void *threadid)
{
	/*
	uint64_t id;
	int cbuffer;

	id = (uint64_t)threadid;
	printf("Thread[%lu] started\n", (uint64_t)threadid);
	while (THR_run) {
		// fitness
		cbuffer = buffer;
		fitness[id] = EVO_fitness(tree[cbuffer][id], gfx_on);
		printf("tree[%lu] fitness %f\n",	id, fitness[id]);
		pthread_barrier_wait(&THR_barrier_1);
		pthread_barrier_wait(&THR_barrier_2);
		// mutate
		EVO_mutate(tree[!cbuffer][id]);
		pthread_barrier_wait(&THR_barrier_3);

	}
	printf("Thread[%lu] finished\n", (uint64_t)threadid);
	// exit
	pthread_exit(NULL);

	*/
	return NULL;
}

void THR_mutex_array_lock(pthread_mutex_t *mutexes, int num)
{
	int i;

	for (i = 0; i < num; i++) {
		pthread_mutex_lock(&(mutexes[i]));
	}
}
void THR_mutex_array_unlock(pthread_mutex_t *mutexes, int num)
{
	for (int i = 0; i < num; i++) {
		pthread_mutex_unlock(&(mutexes[i]));
	}
}

void THR_mutex_array_init(pthread_mutex_t *mutexes, int num)
{
	for (int i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
		pthread_mutex_init(&(mutexes[i]), NULL);
	}
}

void THR_cond_array_init(pthread_cond_t *cond, int num)
{
	for (int i = 0; i < EVO_UNITS_ON_GENERATION; i++) {
		pthread_cond_init(&(cond[i]), NULL);
	}
}
