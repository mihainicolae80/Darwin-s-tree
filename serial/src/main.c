#include "graphics.h"
#include "conf_graphics.h"
#include "conf_tree_gfx.h"
#include "tree.h"
#include "tree_gfx.h"
#include <stdlib.h>
#include <time.h>
#include "evolution.h"
#include "sun.h"
#include "conf_evolution.h"
#include "misc.h"

int main()
{
	bool run;
	SDL_Event event;
	char *tree_auxgenome = "ulbrbuuulrbllbrrblbbbbrlbrrblrbl";
	//char *tree2_auxgenome = "uulbrbullbrbubbrrbllbrbbu";
	treenode_t *tree, *tree2, *tree3 = NULL;
	SDL_Rect leafs[TREEGFX_MAX_NUM_LEAF];
	int index, i;

	srand(time(NULL));
	SUN_init();
	tree_init(&tree);
	//tree_init(&tree2);
	tree_build(tree, &tree_auxgenome);
	//tree_build(tree2, &tree2_auxgenome);

	// offspring
	//tree_free(&tree3);
	//tree_init(&tree3);
	//EVO_crossover(tree3, tree, tree2);

	// get all leafs as rectangles
	tree_get_leafs(	tree, leafs, &index,
			SCREEN_WIDTH / 2,
			SCREEN_HEIGHT - 100,
			0,
			0
	);

	// init
	run = true;
	GFX_init();
	// main loop
	while (run) {
		// handle events
		while (SDL_PollEvent(&event)) {
			SUN_handle_mouse_event(&event);

			if (event.type == SDL_QUIT) {
				run = false;
			}
		}

		// logic

		// render
		GFX_Clear(GFX_WHITE);


		TREEGFX_draw(	tree,
				SCREEN_WIDTH / 2,
				SCREEN_HEIGHT - 100,
				0,
				0
		);

		for (i = 0; i < index; i ++) {
			SDL_SetRenderDrawColor(	_render,
						GFX_COLOR_GET_RED(GFX_RED),
						GFX_COLOR_GET_GREEN(GFX_RED),
						GFX_COLOR_GET_BLUE(GFX_RED),
						GFX_COLOR_GET_ALPHA(GFX_RED)
			);
			SDL_RenderFillRect(_render, &leafs[i]);
		}

		int dx[] = {	-EVO_LEAF_SIZE/3,	0, EVO_LEAF_SIZE/3,
				-EVO_LEAF_SIZE/3, 	0, EVO_LEAF_SIZE/3,
				-EVO_LEAF_SIZE/3,	0, EVO_LEAF_SIZE/3};
		int dy[] = {	-EVO_LEAF_SIZE/3, -EVO_LEAF_SIZE/3,-EVO_LEAF_SIZE/3,
						0,		0,		0,
				 EVO_LEAF_SIZE/3,  EVO_LEAF_SIZE/3, EVO_LEAF_SIZE/3};

		// draw sun exposed blocks
		int lx, ly, k, q, l;
		SDL_Rect subleaf;
		float m;
		float px, py;
		bool collision;
		// light on every leaf
		for (i = 0; i < index; i++) {
			// leaf on every subleaf
			for (q = 0; q < 9; q ++) {
				// subleaf center coordinates
				lx = leafs[i].x + leafs[i].w/2 + dx[q];
				ly = leafs[i].y + leafs[i].h/2 + dy[q];
				// compute intermediary points
				collision = false;
				for(l = 0; l < 5000 && !collision; l++) {
					px = lx + l *((float)SUN_get_x() - lx) / 5000;
					py = ly + l * ((float)SUN_get_y() - ly) / 5000;

					// check collision with all other blocks
					for (k = 0; k < index; k++) {
						if ((k != i)
						&& (px >= leafs[k].x)
						&& (px <= leafs[k].x + leafs[k].w)
						&& (py >= leafs[k].y)
						&& (py <= leafs[k].y + leafs[k].h)
						){
							collision = true;
							break;
						}
					}
				}

				if (!collision) {
					SDL_SetRenderDrawColor(	_render,
						GFX_COLOR_GET_RED(GFX_GREEN),
						GFX_COLOR_GET_GREEN(GFX_GREEN),
						GFX_COLOR_GET_BLUE(GFX_GREEN),
						GFX_COLOR_GET_ALPHA(GFX_GREEN)
					);
					subleaf.x = lx - EVO_LEAF_SIZE / 6;
					subleaf.y = ly - EVO_LEAF_SIZE / 6;
					subleaf.w = EVO_LEAF_SIZE / 3;
					subleaf.h = EVO_LEAF_SIZE / 3;
					SDL_RenderFillRect(_render, &subleaf);
				} else {
					SDL_SetRenderDrawColor(	_render,
						GFX_COLOR_GET_RED(GFX_RED),
						GFX_COLOR_GET_GREEN(GFX_RED),
						GFX_COLOR_GET_BLUE(GFX_RED),
						GFX_COLOR_GET_ALPHA(GFX_RED)
					);
				}

				SDL_RenderDrawLine(_render, lx, ly, SUN_get_x(), SUN_get_y());
			}

		}


		/*
		TREEGFX_draw(	tree2,
				SCREEN_WIDTH * 2 / 4,
				SCREEN_HEIGHT - 100,
				0,
				0
		);

		TREEGFX_draw(	tree3,
				SCREEN_WIDTH * 3 / 4,
				SCREEN_HEIGHT - 100,
				0,
				0
		);
		*/

		TREEGFX_draw_earth();

		SUN_draw();

		GFX_Present();
	}


	tree_free(&tree);
	tree_free(&tree2);
	tree_free(&tree3);
	SDL_Quit();

	return 0;
}
