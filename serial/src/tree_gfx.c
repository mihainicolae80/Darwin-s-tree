#include "tree_gfx.h"
#include "conf_tree_gfx.h"
#include "tree.h"
#include "graphics.h"
#include <math.h>

#define PI 3.14159
#define ABS(x)	((x) < 0 ? -(x) : (x))
#define SIGN(x)	(x < 0 ? -1 : 1)
#define TO_RADIANS(x) ((double)PI * ((ABS((int)(x)) % 360) * SIGN((int)(x))) / 180)

void TREEGFX_draw_earth()
{
	GFX_drawline(	0,
			TREEGFX_EARTH_LEVEL,
			SCREEN_WIDTH,
			TREEGFX_EARTH_LEVEL,
			TREEGFX_EARTH_THICK,
			TREEGFX_EARTH_COLOR
	);
}

void TREEGFX_draw(treenode_t *tree, int x, int y, int angle, int depth)
{
	int delta_x, delta_y, branches = 0;
	float decay;


	// draw each branch recursively
	if (tree) {

		decay = pow(TREEGFX_BRANCH_DECAY, depth);
		if (tree->left) {
			branches ++;
			delta_x = TREEGFX_BRANCH_LEN
				* sin(TO_RADIANS(angle - TREEGFX_BRANCH_ANGLE));
			delta_y = -TREEGFX_BRANCH_LEN
				* cos(TO_RADIANS(angle - TREEGFX_BRANCH_ANGLE));
			delta_x *= decay;
			delta_y *= decay;
			GFX_drawline(	x, y, x + delta_x, y + delta_y,
					TREEGFX_BRANCH_THICK,
					GFX_GREEN
			);
			TREEGFX_draw(	tree->left, x + delta_x, y + delta_y,
					angle - TREEGFX_BRANCH_ANGLE,
					depth + 1
			);
		}

		if (tree->up) {
			branches ++;
			delta_x = TREEGFX_BRANCH_LEN * sin(TO_RADIANS(angle));
			delta_y = -TREEGFX_BRANCH_LEN * cos(TO_RADIANS(angle));
			delta_x *= decay;
			delta_y *= decay;
			GFX_drawline(	x, y, x + delta_x, y + delta_y,
					TREEGFX_BRANCH_THICK,
					GFX_GREEN
			);
			TREEGFX_draw(	tree->up, x + delta_x, y + delta_y,
					angle,
					depth + 1
			);
		}

		if (tree->right) {
			branches ++;
			delta_x = TREEGFX_BRANCH_LEN
				* sin(TO_RADIANS(angle + TREEGFX_BRANCH_ANGLE));
			delta_y = -TREEGFX_BRANCH_LEN
				* cos(TO_RADIANS(angle + TREEGFX_BRANCH_ANGLE));
			delta_x *= decay;
			delta_y *= decay;
			GFX_drawline(	x, y, x + delta_x, y + delta_y,
					TREEGFX_BRANCH_THICK,
					GFX_GREEN
			);
			TREEGFX_draw(	tree->right, x + delta_x, y + delta_y,
					angle + TREEGFX_BRANCH_ANGLE,
					depth + 1
			);
		}

		// draw leaf
		if (0 == branches) {
			boxColor(_render,
				x - TREEGFX_LEAF_SIZE/2,
				y - TREEGFX_LEAF_SIZE/2,
				x + TREEGFX_LEAF_SIZE/2,
				y + TREEGFX_LEAF_SIZE/2,
				TREEGFX_LEAF_COLOR
			);

		// draw knot
		} else {
			filledCircleColor(_render, x, y, TREEGFX_KNOT_SIZE,
					TREEGFX_KNOT_COLOR
			);
		}
	}

}
