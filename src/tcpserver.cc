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

static int make_socket_non_blocking(int fd) {
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

/* public */
void TcpServer::Start() {
	struct sockaddr_in client_addr;
	int		   client_addr_len = sizeof(client_addr);
	if ((server_sockfd_ = CreateSocket()) == -1) {
		LOG_ERR("create socket error: %s\n", strerror(errno));
		exit(-1);
	}
	LOG_DEBUG("created server socket \n");

	RegisterEpoll(server_sockfd_);

	while (1) {

		LOG_DEBUG("waiting connection or message ...\n");
		std::vector< int > active_fds = GotEpollActiveFd();
		for (auto active_fd : active_fds) {
			if (active_fd == server_sockfd_) {
				/* a new connection arrived */
				int client_sockfd = accept(
					server_sockfd_,
					( struct sockaddr* )&client_addr,
					( unsigned int* )&client_addr_len);
				LOG_DEBUG("got client_fd: %d \n",
					  client_sockfd);
				if (client_sockfd < 0) {
					LOG_ERR("accept error : %s\n",
						strerror(errno));
					continue;
				}
				RegisterEpoll(client_sockfd);
			}
			else
				/* a new message arrived */
				worker_.HandleResponse(active_fd);
		}
	}
}
void TcpServer::SingleLoop() {
	server_sockfd_ = CreateSocket();
	struct sockaddr_in client_addr;
	int		   client_addr_len = sizeof(client_addr);

	while (1) {

		LOG_INFO("waiting connection...");

		/* a new connection arrived */
		int		    epollfd = epoll_create(100);
		struct epoll_event  event;
		struct epoll_event* active_events;
		event.data.fd = server_sockfd_;
		event.events  = EPOLLIN;

		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server_sockfd_, &event)
		    != 0) {
			LOG_ERR("epoll add wakeup_fd error: %s",
				strerror(errno));
			exit(-1);
		}
		const int MAX_EVENTS_CNT = 50;
		active_events		 = ( struct epoll_event* )calloc(
			   MAX_EVENTS_CNT, sizeof(epoll_event));
		while (1) {

			int active_events_cnt = epoll_wait(
				epollfd, active_events, MAX_EVENTS_CNT, -1);
			LOG_INFO("epoll wait return \n");
			for (int i = 0; i < active_events_cnt; ++i) {
				LOG_INFO("[%d] active cnt:%d,  active event "
					 "fd: %d\n",
					 i, active_events_cnt,
					 active_events[ i ].data.fd);
				if (active_events[ i ].data.fd
				    == server_sockfd_) {
					int client_sockfd = accept(
						server_sockfd_,
						( struct
						  sockaddr* )&client_addr,
						( unsigned int* )&client_addr_len);
					LOG_INFO("new connection fd: %d\n",
						 client_sockfd);
					if (client_sockfd == -1) {
						LOG_ERR("accpet error: %s",
							strerror(errno));
						continue;
					}
					make_socket_non_blocking(client_sockfd);
					event.data.fd = client_sockfd;
					event.events  = EPOLLIN | EPOLLET;
					epoll_ctl(epollfd, EPOLL_CTL_ADD,
						  client_sockfd, &event);
				}
				else {
					const int   READBUF_LEN = 1024;
					char	readbuf[ READBUF_LEN ];
					int	 read_len;
					std::string msg = "";
					printf("reading msg:\n");
					while ((read_len = recv(
							active_events[ i ]
								.data.fd,
							readbuf, READBUF_LEN,
							MSG_DONTWAIT))
					       > 0) {
						readbuf[ read_len ] = '\0';
						msg += readbuf;
						printf("readbuf:%s\n", readbuf);
					}
				}
			}
		}
	}
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

	printf("bind in %s : %d\n", serv_ip, ntohs(serv_addr.sin_port));
	if (bind(server_sock, ( struct sockaddr* )&serv_addr, serv_addr_len)
	    < 0) {
		LOG_ERR("bind error\n");
		return -1;
	}

	LOG_INFO("bind done\n");

	if (listen(server_sock, backlog_)) {
		printf("listen error\n");
		return -1;
	}
	LOG_INFO("listen done\n");

	// int flags;

	// flags = fcntl(server_sockfd_, F_GETFL, 0);
	// flags |= O_NONBLOCK;

	return server_sock;
}

void TcpServer::StartWorkThreadsPool() {
}
