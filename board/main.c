#include <stdio.h>
#include <stdlib.h>

#include "v4l2.h"

int main(int argc, char *argv[])
{
	int i = 0, j = 0;
	int ret = 0;
	int width = 0, height = 0, fps = 0;
	char *yuv_frame = NULL;

	if (argc < 3) {
		printf("Usage: %s <width> <height> <fps>", argv[0]);
		goto error;
	}

	width = atoi(argv[1]);
	height = atoi(argv[2]);
	fps = atoi(argv[3]);

	if (v4l2_init(width, height, fps) < 0)
		return -1;

	v4l2_start_stream();

	while (1) {
		yuv_frame = v4l2_get_frame();

		v4l2_put_frame();
	}

	v4l2_exit();
	return 0;

error:
	return -1;
}
