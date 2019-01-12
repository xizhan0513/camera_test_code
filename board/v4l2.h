#ifndef __V4L2_H__
#define __V4L2_H__

#include <linux/videodev2.h>

#define VIDEO_DEV "/dev/video0"
#define BUF_NUM 4
#define IOCTL_RETRY 4

typedef struct {
	int len;
	void *addr;
} BUF_SEQ;

struct v4l2_info {
	int v4l2_fd;
	struct v4l2_fmtdesc fmd;
	struct v4l2_format fmt;
	struct v4l2_streamparm parm;
	struct v4l2_requestbuffers req;
	struct v4l2_buffer buffer;
	BUF_SEQ usr_buf[BUF_NUM];
};

int v4l2_init(int, int, int);
int xioctl(int, int, void*);
void v4l2_start_stream();
char* v4l2_get_frame();
void v4l2_put_frame();
void v4l2_exit();

#endif
