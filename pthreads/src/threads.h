#ifndef _THREADS__H_
#define _THREADS__H_

#include <stdbool.h>
#include "conf_evolution.h"
#include <pthread.h>
#include "tree.h"


#define NUM_THREADS	4

typedef void* (*thread_foo_t)(void*);


extern bool THR_run;
extern char *tree_genome[EVO_UNITS_ON_GENERATION];
extern treenode_t *tree[2][EVO_UNITS_ON_GENERATION];
extern int buffer;
extern float fitness[EVO_UNITS_ON_GENERATION];
extern pthread_barrier_t THR_barrier_1, THR_barrier_2, THR_barrier_3;
extern bool gfx_on;



void THR_start_threads(int num_threads, thread_foo_t foo);
void* THR_thread(void *threadid);
void THR_wait_for_threads(void);
void THR_mutex_array_lock(pthread_mutex_t *mutexes, int num);
void THR_mutex_array_unlock(pthread_mutex_t *mutexes, int num);
void THR_mutex_array_init(pthread_mutex_t *mutexes, int num);
void THR_cond_array_init(pthread_cond_t *cond, int num);


#endif
