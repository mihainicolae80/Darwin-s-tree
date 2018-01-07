
#include "graphics.h"
#include <stdio.h>
#include "conf_graphics.h"
#include "misc.h"


#ifdef __SDL__

SDL_Window *_window;
SDL_Renderer *_render;

void GFX_init(void)
{
	_window = NULL;
	_render = NULL;


	_window = SDL_CreateWindow("Darwin's Tree",
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				SDL_WINDOW_SHOWN
	);

	if(_window == NULL) {
		fprintf(stderr, "Could not create Window!\n");
		return;
	}

	_render = SDL_CreateRenderer(_window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if(_render == NULL) {
		fprintf(stderr, "Could not create Renderer\n");
	}
}
#else

void GFX_init(void)
{
	// dummy
}

#endif
