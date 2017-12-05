#ifndef MISC__H_
#define MISC__H_

#include <math.h>

#define PI 3.14159
#define ABS(x)	((x) < 0 ? -(x) : (x))
#define SIGN(x)	(x < 0 ? -1 : 1)
#define BOUND_ANGLE(x) ((ABS((int)(x)) % 360) * SIGN((int)(x)))
#define TO_RADIANS(x) ((double)PI * BOUND_ANGLE(x) / 180)


float distance(float x, float y, float x2, float y2);

#endif
