#include <stdio.h>
#include <SDL.h>

#define TITLE "play yuv video"

int main(int argc, char *argv[])
{
	int width = 0;
	int height = 0;
	Uint32 flags = 0;
	char *yuv_frame = NULL;
	SDL_Rect rect = {0};

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Event event;

	if (argc < 2) {
		printf("Usage: %s <width> <height>\n", argv[0]);
		return -1;
	}

	width  = atoi(argv[1]);
	height = atoi(argv[2]);
	rect.x = 0;
	rect.y = 0;
	rect.w = width;
	rect.h = height;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("sdl init error\n");
		return -1;
	}

	flags = SDL_WINDOW_SHOWN;
	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
	if (window == NULL) {
		printf("SDL_CreateWindow error\n");
		goto error;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		printf("SDL_CreateRenderer error\n");
		goto error;
	}

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (texture == NULL) {
		printf("SDL_CreateTexture error\n");
		goto error;
	}

	while (1) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			break;

		SDL_UpdateTexture(texture, NULL, yuv_frame, width);
		SDL_RenderCopy(renderer, texture, NULL, &rect);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;

error:
	if (window)
		SDL_DestroyWindow(window);
	if (renderer)
		SDL_DestroyRenderer(renderer);
	if (texture)
		SDL_DestroyTexture(texture);

	SDL_Quit();
	return -1;
}
