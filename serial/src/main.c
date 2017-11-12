#include "graphics.h"
#include "conf_graphics.h"
#include "tree.h"
#include "tree_gfx.h"
#include <stdlib.h>
#include <time.h>

int main()
{
	bool run;
	SDL_Event event;
	char *tree_auxgenome = "ullbrbbrrbl";
	char *tree2_auxgenome = "ulbrbuuulrbllbrrblbbbbrlbrrblrbl";
	char *tree3_auxgenome = "uulbrbullbrbubbrrbllbrbbu";
	treenode_t *tree, *tree2, *tree3;

	srand(time(NULL));

	tree_init(&tree);
	tree_init(&tree2);
	tree_init(&tree3);

	tree_build(tree, &tree_auxgenome);
	tree_build(tree2, &tree2_auxgenome);
	tree_build(tree3, &tree3_auxgenome);

	// init
	run = true;
	GFX_init();
	// main loop
	while (run) {
		// handle events
		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {

				run = false;
			}
		}

		// logic

		// render
		GFX_Clear(GFX_WHITE);

		TREEGFX_draw(	tree,
				SCREEN_WIDTH / 4,
				SCREEN_HEIGHT - 100,
				0,
				0
		);

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

		TREEGFX_draw_earth();
		GFX_Present();
	}


	tree_free(&tree);
	tree_free(&tree2);
	tree_free(&tree3);
	SDL_Quit();

	return 0;
}
