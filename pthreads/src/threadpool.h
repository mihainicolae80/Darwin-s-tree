#ifndef THREADPOOL_H__
#define THREADPOOL_H__

#include "queue.h"

void TPOOL_init(void);
void TPOOL_start(int num_threads);
void TPOOL_enqueue(task_t task);
void TPOOL_wait_for_all(void);
void TPOOL_join(void);

#endif
