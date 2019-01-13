#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#include "net.h"

struct net_info net_var = {0};

int net_init(char *ip, char *port)
{
	net_var.clnt_addr_size = sizeof(struct sockaddr_in);

	net_var.serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	net_var.serv_addr.sin_family = AF_INET;
	net_var.serv_addr.sin_addr.s_addr = inet_addr(ip);
	net_var.serv_addr.sin_port = htons(atoi(port));

	bind(net_var.serv_sock, (struct sockaddr*)&net_var.serv_addr, sizeof(net_var.serv_addr));

	listen(net_var.serv_sock, LISTEN_NUM);

	net_var.clnt_sock = accept(net_var.serv_sock, (struct sockaddr*)&net_var.clnt_addr, &net_var.clnt_addr_size);

	return 0;
}

int get_frame(char *yuv_frame, int frame_size)
{
	int ret = 0;
	char *tmp_frame = yuv_frame;

	while (frame_size > 0) {
		ret = read(net_var.clnt_sock, tmp_frame, BUF_SIZE);
		if (ret < 0)
			return -1;

		frame_size -= ret;
		tmp_frame += ret;
	}

	return 0;
}

void net_exit()
{
	close(net_var.serv_sock);
	close(net_var.clnt_sock);
	return ;
}
