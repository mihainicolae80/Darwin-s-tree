#ifndef MISC__H_
#define MISC__H_

#include <math.h>
#include "conf_evolution.h"

#define __SDL__


typedef struct {
	int x, y, w, h;
}rect_t;




#define PI 3.14159
#define ABS(x)	((x) < 0 ? -(x) : (x))
#define SIGN(x)	(x < 0 ? -1 : 1)
#define BOUND_ANGLE(x) ((ABS((int)(x)) % 360) * SIGN((int)(x)))
#define TO_RADIANS(x) ((double)PI * BOUND_ANGLE(x) / 180)


float distance(float x, float y, float x2, float y2);


extern int rand_val[EVO_UNITS_ON_GENERATION][1000];
extern int rand_index[EVO_UNITS_ON_GENERATION];

void MISC_gen_rand(void);

#endif
