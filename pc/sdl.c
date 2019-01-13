#include <stdio.h>

#include "sdl.h"

struct sdl_info sdl_var = {0};

int sdl_init(int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("sdl init error\n");
		return -1;
	}

	sdl_var.window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (sdl_var.window == NULL) {
		printf("SDL_CreateWindow error\n");
		goto error;
	}

	sdl_var.renderer = SDL_CreateRenderer(sdl_var.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (sdl_var.renderer == NULL) {
		printf("SDL_CreateRenderer error\n");
		goto error;
	}

	sdl_var.texture = SDL_CreateTexture(sdl_var.renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (sdl_var.texture == NULL) {
		printf("SDL_CreateTexture error\n");
		goto error;
	}

	return 0;

error:
	if (sdl_var.window)
		SDL_DestroyWindow(sdl_var.window);
	if (sdl_var.renderer)
		SDL_DestroyRenderer(sdl_var.renderer);
	if (sdl_var.texture)
		SDL_DestroyTexture(sdl_var.texture);

	SDL_Quit();
	return -1;
}

void sdl_exit()
{
	if (sdl_var.window)
		SDL_DestroyWindow(sdl_var.window);
	if (sdl_var.renderer)
		SDL_DestroyRenderer(sdl_var.renderer);
	if (sdl_var.texture)
		SDL_DestroyTexture(sdl_var.texture);

	SDL_Quit();

	return ;
}

void sdl_show(char *frame, int width, int height)
{
	SDL_Rect rect = {0};
	rect.x = 0;
	rect.y = 0;
	rect.w = width;
	rect.h = height;

	if (SDL_UpdateTexture(sdl_var.texture, NULL, frame, width) < 0) {
		printf("SDL_UpdateTexture error\n");
		return ;
	}

	if (SDL_RenderCopy(sdl_var.renderer, sdl_var.texture, NULL, &rect) < 0) {
		printf("SDL_RenderCopy error\n");
		return ;
	}

	SDL_RenderPresent(sdl_var.renderer);
	return ;
}
