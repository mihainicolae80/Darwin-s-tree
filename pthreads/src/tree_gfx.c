#include "tree_gfx.h"
#include "conf_tree_gfx.h"
#include "conf_evolution.h"
#include "tree.h"
#include "graphics.h"
#include <math.h>
#include "misc.h"

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

#ifdef __SDL__

void TREEGFX_draw(treenode_t *tree, int x, int y, int angle, int depth)
{
	int branches = 0;
	float delta_x, delta_y;
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
				x - EVO_LEAF_SIZE/2,
				y - EVO_LEAF_SIZE/2,
				x + EVO_LEAF_SIZE/2,
				y + EVO_LEAF_SIZE/2,
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
#else
void TREEGFX_draw(treenode_t *tree, int x, int y, int angle, int depth)
{
	// dummy
}
#endif
