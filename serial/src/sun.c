#include "sun.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "graphics.h"
#include "conf_sun.h"
#include <stdbool.h>
#include "misc.h"

static struct {
	int x, y;
	bool selected;
}_data;

void SUN_init()
{
	_data.x = 0;
	_data.y = 0;
	_data.selected = false;
}

void SUN_draw()
{
	filledCircleColor( _render, _data.x, _data.y, SUN_RADIUS, SUN_COLOR);
}

void SUN_handle_mouse_event(SDL_Event *event)
{
	if (SDL_MOUSEBUTTONDOWN == event->type)
	{

		if (distance(event->button.x, event->button.y, _data.x, _data.y) <= SUN_RADIUS) {
			_data.selected = true;
		}
	} else if (SDL_MOUSEBUTTONUP == event->type)
	{
		if (distance(event->button.x, event->button.y, _data.x, _data.y) <= SUN_RADIUS) {
			_data.selected = false;
		}
	} else if (SDL_MOUSEMOTION == event->type) {
		if (_data.selected) {
			_data.x = event->motion.x;
			_data.y = event->motion.y;
		}
	}
}

int SUN_get_x()
{
	return _data.x;
}

int SUN_get_y()
{
	return _data.y;
}

void SUN_set_x(int x)
{
	_data.x = x;
}

void SUN_set_y(int y)
{
	_data.y = y;
}
