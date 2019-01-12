#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "v4l2.h"

struct v4l2_info v4l2_var = {0};

int xioctl(int fd, int cmd, void *arg)
{
	int ret = 0;
	int retry = IOCTL_RETRY;

	do {
		ret = ioctl(fd, cmd, arg);
	} while(ret && retry-- && ((errno == EINTR) || (errno == EAGAIN) || (errno == ETIMEDOUT)));

	if (ret && (retry <= 0)) {
		printf("xioctl error\n");
		return ret;
	}

	return ret;
}

int v4l2_init(int width, int height, int fps)
{
	int i = 0;

	v4l2_var.v4l2_fd = open(VIDEO_DEV, O_RDWR);
	if (v4l2_var.v4l2_fd < 0) {
		perror("open video device:\n");
		goto open_error;
	}

	v4l2_var.fmd.index = 0;
	v4l2_var.fmd.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	printf("\n");
	while (xioctl(v4l2_var.v4l2_fd, VIDIOC_ENUM_FMT, &v4l2_var.fmd) >= 0) {
		v4l2_var.fmd.index++;
		printf("index = %d, description = %s\n", v4l2_var.fmd.index, v4l2_var.fmd.description);
	}

	v4l2_var.fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v4l2_var.fmt.fmt.pix.width = width;
	v4l2_var.fmt.fmt.pix.height = height;
	v4l2_var.fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;

	if (xioctl(v4l2_var.v4l2_fd, VIDIOC_S_FMT, &v4l2_var.fmt)) {
		printf("video ioctl set format error\n");
		goto error;
	}

	v4l2_var.parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v4l2_var.parm.parm.capture.timeperframe.numerator = 1;
	v4l2_var.parm.parm.capture.timeperframe.denominator = fps;

	if (xioctl(v4l2_var.v4l2_fd, VIDIOC_S_PARM, &v4l2_var.parm)) {
		printf("video ioctl set parm error\n");
		goto error;
	}

	v4l2_var.req.count = BUF_NUM;
	v4l2_var.req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v4l2_var.req.memory = V4L2_MEMORY_MMAP;

	if (xioctl(v4l2_var.v4l2_fd, VIDIOC_REQBUFS, &v4l2_var.req)) {
		printf("video ioctl reqbufs error\n");
		goto error;
	}

	for (i = 0; i < BUF_NUM; i++) {
		v4l2_var.buffer.index = i;
		v4l2_var.buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		v4l2_var.buffer.memory = V4L2_MEMORY_MMAP;

		if (xioctl(v4l2_var.v4l2_fd, VIDIOC_QUERYBUF, &v4l2_var.buffer) < 0) {
			printf("video ioctl querybuf error\n");
			goto error;
		}

		v4l2_var.usr_buf[i].len = v4l2_var.buffer.bytesused;
		v4l2_var.usr_buf[i].addr = mmap(NULL, v4l2_var.buffer.length, PROT_READ | PROT_WRITE, MAP_SHARED, v4l2_var.v4l2_fd, v4l2_var.buffer.m.offset);
		if (v4l2_var.usr_buf[i].addr == NULL) {
			printf("mmap error\n");
			goto error;
		}
	}

	for (i = 0; i < BUF_NUM; i++) {
		v4l2_var.buffer.index = i;
		v4l2_var.buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		v4l2_var.buffer.memory = V4L2_MEMORY_MMAP;

		if (xioctl(v4l2_var.v4l2_fd, VIDIOC_QBUF, &v4l2_var.buffer) < 0) {
			printf("video ioctl qbuf error\n");
			goto error;
		}
	}

	return 0;

error:
	close(v4l2_var.v4l2_fd);
open_error:
	return -1;
}

void v4l2_start_stream()
{
	int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (xioctl(v4l2_var.v4l2_fd, VIDIOC_STREAMON, &type) < 0)
		printf("video ioctl streamon error\n");

	return ;
}

char* v4l2_get_frame()
{
	v4l2_var.buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v4l2_var.buffer.memory = V4L2_MEMORY_MMAP;

	if (xioctl(v4l2_var.v4l2_fd, VIDIOC_DQBUF, &v4l2_var.buffer) >= 0)
		return v4l2_var.usr_buf[v4l2_var.buffer.index].addr;

	return NULL;
}

void v4l2_put_frame()
{
	xioctl(v4l2_var.v4l2_fd, VIDIOC_QBUF, &v4l2_var.buffer);
	return ;
}

void v4l2_exit()
{
	int i = 0;
	int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	xioctl(v4l2_var.v4l2_fd, VIDIOC_STREAMOFF, &type);

	for (i = 0; i < BUF_NUM; i++) {
		munmap(&v4l2_var.usr_buf[i].addr, v4l2_var.usr_buf[i].len);
	}

	close(v4l2_var.v4l2_fd);

	return ;
}
