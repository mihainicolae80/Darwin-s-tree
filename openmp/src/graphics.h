#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint32_t GFX_color_t;

extern SDL_Window *_window;
extern SDL_Renderer *_render;

// ========= COLORS =========
#define GFX_WHITE	0xffffffff
#define GFX_BLACK	0xff000000
#define GFX_RED		0xffff0000
#define GFX_GREEN	0xff00ff00
#define GFX_BLUE	0xff0000ff
#define GFX_COLOR_GET_ALPHA(x)	(x >> 24)
#define GFX_COLOR_GET_RED(x) 	((x >> 16) & 0xff)
#define GFX_COLOR_GET_GREEN(x) ((x >> 8) & 0xff)
#define GFX_COLOR_GET_BLUE(x) 	(x & 0xff)


void GFX_init(void);

static inline void GFX_SetDrawColor(GFX_color_t color)
{
	SDL_SetRenderDrawColor(	_render,
				GFX_COLOR_GET_RED(color),
				GFX_COLOR_GET_GREEN(color),
				GFX_COLOR_GET_BLUE(color),
				GFX_COLOR_GET_ALPHA(color)
	);
}

static inline void GFX_drawline(int x, int y, int x2, int y2, float thick, GFX_color_t color)
{
	GFX_SetDrawColor(color);
	thickLineRGBA(	_render, x, y, x2, y2, thick,
			GFX_COLOR_GET_RED(color),
			GFX_COLOR_GET_GREEN(color),
			GFX_COLOR_GET_BLUE(color),
			GFX_COLOR_GET_ALPHA(color)
	);
}

static inline void GFX_Clear(GFX_color_t color)
{
	GFX_SetDrawColor(color);
	SDL_RenderClear(_render);
}

static inline void GFX_Present()
{
	SDL_RenderPresent(_render);
}




/*
	RenderSetScale
*/

#endif // __GRAPHICS_H__
