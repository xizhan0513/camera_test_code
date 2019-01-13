#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct net_info net_var = {0};

int net_init(char *ip, char *port)
{
	int ret = 0;
	net_var.clnt_addr_size = sizeof(struct sockaddr_in);

	net_var.serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	net_var.serv_addr.sin_family = AF_INET;
	net_var.serv_addr.sin_addr.s_addr = inet_addr(ip);
	net_var.serv_addr.sin_port = htons(atoi(port));

	bind(net_var.serv_sock, (struct sockaddr*)&net_var.serv_addr, sizeof(net_var.serv_addr));

	listen(net_var.serv_sock, LINSTEN_NUM);

	net_var.clnt_sock = accept(net_var.serv_sock, (struct sockaddr*)&net_var.clnt_addr, &net_var.clnt_addr_size);

	return 0;
}

void send_frame(char *frame)
{


}
