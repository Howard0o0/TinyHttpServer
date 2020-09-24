#include "sockettool.h"
#include "log.h"
#include <arpa/inet.h>
#include <boost/log/trivial.hpp>
#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace nethelper;

/* public methods */
int SocketTool::CreateListenSocket(int port, int backlog, bool block) {
	int server_sock = CreateSocket(block);
	if (server_sock < 0)
		return -1;

	struct sockaddr_in serv_addr;
	int		   serv_addr_len = sizeof(serv_addr);
	serv_addr.sin_family		 = AF_INET;
	serv_addr.sin_addr.s_addr	 = htonl(INADDR_ANY);
	serv_addr.sin_port		 = htons(port);

	char serv_ip[ INET_ADDRSTRLEN ];
	if (inet_ntop(AF_INET, &serv_addr.sin_addr, serv_ip, sizeof(serv_ip)) == NULL) {
		LOG(error) << "inet_ntop error";
		close(server_sock);
		return -1;
	}

	LOG(info) << "bind in " << serv_ip << " : " << ntohs(serv_addr.sin_port);
	if (bind(server_sock, ( struct sockaddr* )&serv_addr, serv_addr_len) < 0) {
		LOG(error) << "bind error";
		LOG(error) << strerror(errno);
		return -1;
	}

	LOG(debug) << "bind done";

	if (listen(server_sock, backlog) < 0) {
		LOG(error) << "listen error";
		LOG(error) << strerror(errno);
		return -1;
	}
	LOG(debug) << "listen done";

	return server_sock;
}

std::string SocketTool::ReadMessage(int socketfd) {
	const int   READBUF_LEN = 1024;
	char	    readbuf[ READBUF_LEN ];
	int	    read_len;
	std::string msg = "";
	while ((read_len = recv(socketfd, readbuf, READBUF_LEN, MSG_DONTWAIT)) > 0) {
		readbuf[ read_len ] = '\0';
		msg += readbuf;
		// printf("readbuf:%s\n", readbuf);
	}
	return msg;
}

int SocketTool::ConnectToServer(const std::string& server_ip, uint16_t server_port, bool block) {
	int socketfd = CreateSocket(block);
	if (socketfd < 0) {
		LOG(error) << "connect to server " << server_ip << ":" << server_port
			   << " failed,exit";
		exit(-1);
	}

	struct sockaddr_in remote_addr;		       //服务器端网络地址结构体
	memset(&remote_addr, 0, sizeof(remote_addr));  //数据初始化--清零
	remote_addr.sin_family	    = AF_INET;	       //设置为IP通信
	remote_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());  //服务器IP地址
	remote_addr.sin_port	    = htons(server_port);	     //服务器端口号

	if (connect(socketfd, ( struct sockaddr* )&remote_addr, sizeof(struct sockaddr)) < 0) {
		LOG(error) << "connect to server " << server_ip << ":" << server_port
			   << " failed,exit";
		LOG(error) << strerror(errno);
		exit(-1);
	}

	return socketfd;
}

SockAddress SocketTool::ParseSockAddr(const struct sockaddr_in* sockaddr) {
	SockAddress addr;
	char	    buff[ INET_ADDRSTRLEN + 1 ] = { 0 };
	inet_ntop(AF_INET, &sockaddr->sin_addr, buff, INET_ADDRSTRLEN);
	addr.port = ntohs(sockaddr->sin_port);
	addr.ip	  = std::string(buff);
	return addr;
}
/* end of public methods */

/* private methods */
void SocketTool::SetSocketNonblocking(int sockfd) {
	int opts;

	opts = fcntl(sockfd, F_GETFL);
	if (opts < 0) {
		LOG(error) << "fcntl F_GETFL error :" << errno;
		exit(1);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(sockfd, F_SETFL, opts) < 0) {
		LOG(error) << "fcntl F_SETFL error : " << errno;
		exit(1);
	}
}

bool SocketTool::SetSocketReuse(int socket_fd) {
	int opt = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, ( const void* )&opt,
		       sizeof(opt))
	    < 0)
		return false;
	return true;
}

int SocketTool::CreateSocket(bool block) {

	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

	if (SetSocketReuse(socketfd) == false) {
		LOG(error) << "set addr reuse failed";
		LOG(error) << strerror(errno);
		return -1;
	}
	if (!block)
		SocketTool::SetSocketNonblocking(socketfd);

	return socketfd;
}
/* end of private methods */
