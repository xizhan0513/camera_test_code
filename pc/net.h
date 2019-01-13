#ifndef __NET_H__
#define __NET_H__

#define LISTEN_NUM 10

struct net_info {
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
};

int net_init(char*, char*);
void send_frame(char*);

#endif
