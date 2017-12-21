#include "queue.h"
#include <stdio.h>

#define _SIZE 100

task_t _data[_SIZE];
int head, tail, size;


void QUEUE_init()
{
	head = 0;
	tail = 0;
	size = 0;
}

void QUEUE_enqueue(task_t task)
{
	// wait until the queue gets empty
	while (size == _SIZE)
		;
	_data[head] = task;
	head = (head + 1) % _SIZE;
	size ++;

}

task_t QUEUE_dequeue(void)
{
	task_t task;

	task.callback = NULL;

	if (size) {
		task = _data[tail];
		tail = (tail + 1) % _SIZE;
		size --;
	}

	return task;
}

int QUEUE_size()
{
	return size;
}
