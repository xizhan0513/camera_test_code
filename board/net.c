#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#include "net.h"

struct net_info net_var = {0};

int net_init(char *ip, char *port)
{
	int ret = 0;
	net_var.clnt_sock = socket(AF_INET, SOCK_STREAM, 0);

	net_var.serv_addr.sin_family = AF_INET;
	net_var.serv_addr.sin_addr.s_addr = inet_addr(ip);
	net_var.serv_addr.sin_port = htons(atoi(port));

	ret = connect(net_var.clnt_sock, (struct sockaddr*)&net_var.serv_addr, sizeof(net_var.serv_addr));
	if (ret != 0) {
		printf("connect error\n");
		return -1;
	}

	return 0;
}

int send_frame(char *frame, int width, int height)
{
	if (write(net_var.clnt_sock, frame, width * height * 3 / 2) <= 0)
		return -1;
	return 0;
}

void net_exit()
{
	close(net_var.clnt_sock);
	return ;
}
