#include "tcpserver.h"
#include "connection.h"
#include "lockfreethreadpool.h"
#include "log.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace ths;

static int SetSocketNonblock(int fd) {
	int flags, s;
	// 获取当前flag
	flags = fcntl(fd, F_GETFL, 0);
	if (-1 == flags) {
		perror("Get fd status");
		return -1;
	}

	flags |= O_NONBLOCK;

	// 设置flag
	s = fcntl(fd, F_SETFL, flags);
	if (-1 == s) {
		perror("Set fd status");
		return -1;
	}
	return 0;
}

void TcpServer::SetSocketReuse(int socket_fd) {
	int opt = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, ( const void* )&opt,
		   sizeof(opt));
}

/* public */
void TcpServer::Start() {
	struct sockaddr_in client_addr;
	int		   client_addr_len = sizeof(client_addr);
	if ((server_sockfd_ = CreateSocket()) == -1) {
		LOG_ERR("create socket error: %s\n", strerror(errno));
		exit(-1);
	}

	while (1) {
		// int client_sockfd =
		// 	accept(server_sockfd_, ( struct sockaddr* )&client_addr,
		// 	       ( unsigned int* )&client_addr_len);
		int client_sockfd = accept(server_sockfd_, NULL, NULL);
		if (client_sockfd < 0) {
			LOG_ERR("accept error : %s\n", strerror(errno));
			continue;
		}
		LOG_DEBUG("accept a new client_fd:%d \n", client_sockfd);
		worker_.HandleResponse(client_sockfd);
	}
}

void TcpServer::SetOnMsgCallback(const OnMsgCallback& cb) {
	worker_.SetOnMessageCallback(cb);
}
/* private */

void TcpServer::InitialEpollinfo() {
	epollinfo_.epollfd       = epoll_create(100);
	epollinfo_.event.events  = EPOLLIN | EPOLLET;
	epollinfo_.active_events = ( struct epoll_event* )calloc(
		EpollInfo::MAX_EVENTS_CNT, sizeof(epoll_event));
}
void TcpServer::RegisterEpoll(int fd) {

	epollinfo_.event.data.fd = fd;

	if (epoll_ctl(epollinfo_.epollfd, EPOLL_CTL_ADD, fd, &epollinfo_.event)
	    != 0) {
		LOG_ERR("epoll add fd error: %s \n", strerror(errno));
		// exit(-1);
	}
}

void TcpServer::DelEpoll(int fd) {

	epollinfo_.event.data.fd = fd;

	if (epoll_ctl(epollinfo_.epollfd, EPOLL_CTL_DEL, fd, NULL) != 0) {
		LOG_ERR("epoll add fd error: %s \n", strerror(errno));
		// exit(-1);
	}
}

std::vector< int > TcpServer::GotEpollActiveFd() {
	int active_events_cnt =
		epoll_wait(epollinfo_.epollfd, epollinfo_.active_events,
			   EpollInfo::MAX_EVENTS_CNT, -1);
	std::vector< int > active_fds;
	for (int i = 0; i < active_events_cnt; ++i)
		active_fds.push_back(epollinfo_.active_events->data.fd);
	return active_fds;
}
int TcpServer::CreateSocket() {
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serv_addr;
	int		   serv_addr_len = sizeof(serv_addr);
	serv_addr.sin_family		 = AF_INET;
	serv_addr.sin_addr.s_addr	= htonl(INADDR_ANY);
	serv_addr.sin_port		 = htons(port_);

	char serv_ip[ INET_ADDRSTRLEN ];
	if (inet_ntop(AF_INET, &serv_addr.sin_addr, serv_ip, sizeof(serv_ip))
	    == NULL) {
		LOG_ERR("inet_ntop error\n");
		close(server_sock);
		return -1;
	}

	SetSocketReuse(server_sock);

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
