#include "tcpserver.h"
#include "lockfreethreadpool.h"
#include "log.h"
#include "sockettool.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace nethelper;

void TcpServer::SetSocketReuse(int socket_fd) {
	int opt = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, ( const void* )&opt,
		   sizeof(opt));
}

/* public */
void TcpServer::Start() {
	struct sockaddr_in client_addr;
	int		   client_addr_len = sizeof(client_addr);
	if ((server_sockfd_ = CreateSocket(false)) == -1) {
		LOG_ERR("create socket error: %s\n", strerror(errno));
		exit(-1);
	}

	EpollInfo epollinfo;
	EpollTool::InitialEpollinfo(epollinfo);
	EpollTool::RegisterEpoll(server_sockfd_, epollinfo, EPOLLIN);

	while (1) {
		int	   connfd  = -1;
		static int conncnt = 0;

		int active_events_cnt =
			epoll_wait(epollinfo.epollfd, epollinfo.active_events,
				   EpollInfo::MAX_EVENTS_CNT, -1);
		while (1) {
			LOG_INFO("conn cnt : %d\n", conncnt);
			connfd = accept(server_sockfd_, NULL, NULL);
			if (connfd > 0) {
				++conncnt;
				conncnt = (conncnt + 1) % INT32_MAX;
				LOG_INFO("accept a new client_fd:%d \n",
					 connfd);
				worker_.HandleResponse(connfd);
			}
			else if (errno == EAGAIN)
				break;
		}
		// while ((connfd = accept(server_sockfd_, NULL, NULL)) > 0) {
		// 	LOG_INFO("accept a new client_fd:%d \n", connfd);
		// 	worker_.HandleResponse(connfd);
		// }
	}
}

void TcpServer::SetOnMsgCallback(const OnMsgCallback& cb) {
	worker_.SetOnMessageCallback(cb);
}
/* private */

int TcpServer::CreateSocket(bool block) {
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serv_addr;
	int		   serv_addr_len = sizeof(serv_addr);
	serv_addr.sin_family		 = AF_INET;
	serv_addr.sin_addr.s_addr	 = htonl(INADDR_ANY);
	serv_addr.sin_port		 = htons(port_);

	char serv_ip[ INET_ADDRSTRLEN ];
	if (inet_ntop(AF_INET, &serv_addr.sin_addr, serv_ip, sizeof(serv_ip))
	    == NULL) {
		LOG_ERR("inet_ntop error\n");
		close(server_sock);
		return -1;
	}

	SetSocketReuse(server_sock);
	if (!block)
		SocketTool::SetSocketNonblocking(server_sock);

	printf("bind in %s : %d\n", serv_ip, ntohs(serv_addr.sin_port));
	if (bind(server_sock, ( struct sockaddr* )&serv_addr, serv_addr_len)
	    < 0) {
		LOG_ERR("bind error\n");
		return -1;
	}

	LOG_DEBUG("bind done\n");

	if (listen(server_sock, backlog_)) {
		printf("listen error\n");
		return -1;
	}
	LOG_DEBUG("listen done\n");

	return server_sock;
}
