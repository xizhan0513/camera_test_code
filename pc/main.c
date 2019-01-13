#include <stdio.h>
#include <stdlib.h>

#include "sdl.h"
#include "net.h"

int main(int argc, char *argv[])
{
	int width = 0;
	int height = 0;
	int frame_size = 0;
	char *yuv_frame = NULL;
	SDL_Event event;

	if (argc < 5) {
		printf("Usage: %s <width> <height> <pc ip> <port>\n", argv[0]);
		return -1;
	}

	width  = atoi(argv[1]);
	height = atoi(argv[2]);
	frame_size = width * height * 3 / 2;

	if (net_init(argv[3], argv[4]) < 0)
		goto error;
	if (sdl_init(width, height) < 0) {
		net_exit();
		goto error;
	}

	yuv_frame = (char*)malloc(frame_size);
	if (yuv_frame == NULL) {
		printf("malloc error\n");
		net_exit();
		goto error;
	}

	while (1) {
		if (get_frame(yuv_frame, frame_size) < 0)
			break;

		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			break;

		sdl_show(yuv_frame, width, height);
	}

	free(yuv_frame);
	net_exit();
	sdl_exit();
	return 0;

error:
	sdl_exit();
	return -1;
}
