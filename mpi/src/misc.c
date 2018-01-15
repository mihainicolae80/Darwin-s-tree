#include "misc.h"
#include <stdlib.h>


int rand_val[EVO_UNITS_ON_GENERATION][1000];
int rand_index[EVO_UNITS_ON_GENERATION];

float distance(float x, float y, float x2, float y2)
{
	return sqrt(pow(x - x2, 2) + pow(y - y2, 2));
}

void MISC_gen_rand(void)
{
	int i, j;

	// reset indexes
	for (i = 0; i < EVO_UNITS_ON_GENERATION; i++)
		rand_index[i] = 0;
	// generate randoms
	for (i = 0; i < EVO_UNITS_ON_GENERATION; i++)
		for (j = 0; j < 1000; j++)
			rand_val[i][j] = rand();
}
