#ifndef SUN__H_
#define SUN__H_

/*

 ========= DEPRECATED ==========
*/

#include <SDL2/SDL.h>

void SUN_init();
void SUN_draw();
void SUN_handle_mouse_event(SDL_Event *event);
int SUN_get_x();
int SUN_get_y();
void SUN_set_x(int x);
void SUN_set_y(int y);

#endif
