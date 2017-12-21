#ifndef QUEUE__H_
#define QUEUE__H_

typedef void (*callback_t)(int);

typedef struct {
	callback_t callback;
	int index;
} task_t;

void QUEUE_init(void);
void QUEUE_enqueue(task_t task);
task_t QUEUE_dequeue(void);
int QUEUE_size(void);

#endif
