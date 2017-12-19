#include "threadpool.h"
#include "threads.h"
#include "queue.h"


static pthread_mutex_t _mux_queue, _mux_cond;
static pthread_mutex_t _mux_aux, _mux_wait_all;
static pthread_cond_t _cond_queue, _cond_wait_all;
static int _busy_threads;


static task_t _dequeue()
{
	task_t task;

	pthread_mutex_lock(&_mux_aux);
	task = QUEUE_dequeue();
	pthread_mutex_unlock(&_mux_aux);

	return task;
}

void TPOOL_enqueue(task_t task)
{
	fflush(stdout);
	pthread_mutex_lock(&_mux_aux);
	fflush(stdout);
	QUEUE_enqueue(task);
	pthread_mutex_unlock(&_mux_aux);

	// wake up another worker
	if ((QUEUE_size() > 2) || (_busy_threads == 0)) {
		pthread_mutex_lock(&_mux_cond);
		pthread_cond_signal(&_cond_queue);
		pthread_mutex_unlock(&_mux_cond);
	}
}

static void *_thread(void *threadid)
{
	uint64_t id;
	task_t task;


	id = (uint64_t)threadid;
	printf("Thread[%lu] started\n", id);
	while (THR_run) {
		// wait for job
		pthread_mutex_lock(&_mux_cond);
		pthread_cond_wait(&_cond_queue, &_mux_cond);
		// thread busy
		_busy_threads ++;
		pthread_mutex_unlock(&_mux_cond);
		// printf("threads=%d\n", _busy_threads);
		while (THR_run)
		{
			task = _dequeue();
			// no task available
			if (!task.callback)
				break;
			// do work
			task.callback(task.index);
		}
		// worker goes to sleep
		pthread_mutex_lock(&_mux_cond);
		// thread busy
		_busy_threads --;
		if (!_busy_threads) {
			pthread_mutex_lock(&_mux_wait_all);
			pthread_cond_signal(&_cond_wait_all);
			pthread_mutex_unlock(&_mux_wait_all);
		}
		pthread_mutex_unlock(&_mux_cond);
	}

	printf("Thread[%lu] finished\n", id);
	// exit
	pthread_exit(NULL);
	return NULL;
}

void TPOOL_init(void)
{
	QUEUE_init();
	pthread_mutex_init(&_mux_cond, NULL);
	pthread_mutex_init(&_mux_aux, NULL);
	pthread_mutex_init(&_mux_queue, NULL);
	pthread_mutex_init(&_mux_wait_all, NULL);
	pthread_cond_init(&_cond_queue, NULL);
	pthread_cond_init(&_cond_wait_all, NULL);
}

void TPOOL_start(int num_threads)
{
	_busy_threads = 0;
	THR_run = true;
	THR_start_threads(num_threads, _thread);
}

void TPOOL_wait_for_all(void)
{
	while ((_busy_threads > 0) || (QUEUE_size() > 0))
	{
		pthread_mutex_lock(&_mux_wait_all);
		pthread_cond_wait(&_cond_wait_all, &_mux_wait_all);
		pthread_mutex_unlock(&_mux_wait_all);
	}

}

void TPOOL_join(void)
{
	THR_run = false;
	pthread_mutex_lock(&_mux_cond);
	pthread_cond_broadcast(&_cond_queue);
	pthread_mutex_unlock(&_mux_cond);
}
