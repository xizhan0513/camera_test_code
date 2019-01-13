#ifndef __SDL_H__
#define __SDL_H__

#include <SDL.h>

#define TITLE "play yuv video"

struct sdl_info {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
};

int sdl_init(int, int);
void sdl_exit();
void sdl_show(char*, int, int);

#endif
