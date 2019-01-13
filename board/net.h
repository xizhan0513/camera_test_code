#ifndef __NET_H__
#define __NET_H__

#include <arpa/inet.h>

#define LISTEN_NUM 10

struct net_info {
	int clnt_sock;
	struct sockaddr_in serv_addr;
};

int net_init(char*, char*);
void net_exit();
int send_frame(char*, int, int);

#endif
