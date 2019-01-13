#include <stdio.h>
#include <stdlib.h>

#include "v4l2.h"
#include "net.h"

int main(int argc, char *argv[])
{
	int ret = 0;
	int width = 0, height = 0, fps = 0;
	char *yuv_frame = NULL;

	if (argc < 6) {
		printf("Usage: %s <width> <height> <fps> <pc ip> <port>\n", argv[0]);
		return -1;
	}

	width  = atoi(argv[1]);
	height = atoi(argv[2]);
	fps    = atoi(argv[3]);

	if (v4l2_init(width, height, fps) < 0)
		return -1;

	if (net_init(argv[4], argv[5]) < 0)
		goto error;

	v4l2_start_stream();

	while (1) {
		yuv_frame = v4l2_get_frame();
		ret = send_frame(yuv_frame, width, height);
		if (ret < 0)
			break;
		v4l2_put_frame();
	}

	net_exit();
	v4l2_exit();
	return 0;

error:
	v4l2_exit();
	return -1;
}
