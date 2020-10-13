#include "sockettool.h"
#include "log.h"
#include <arpa/inet.h>
#include <boost/log/trivial.hpp>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <regex>
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
		return CREATE_SOCKET_FAILED;

	struct sockaddr_in serv_addr;
	int		   serv_addr_len = sizeof(serv_addr);
	serv_addr.sin_family		 = AF_INET;
	serv_addr.sin_addr.s_addr	 = htonl(INADDR_ANY);
	serv_addr.sin_port		 = htons(port);

	char serv_ip[ INET_ADDRSTRLEN ];
	if (inet_ntop(AF_INET, &serv_addr.sin_addr, serv_ip, sizeof(serv_ip)) == NULL) {
		LOG(error) << "inet_ntop error";
		close(server_sock);
		return CONVERT_IPADDR_TO_BINARY_FAILED;
	}

	LOG(info) << "bind in " << serv_ip << " : " << ntohs(serv_addr.sin_port);
	if (bind(server_sock, ( struct sockaddr* )&serv_addr, serv_addr_len) < 0) {
		LOG(error) << "bind error";
		LOG(error) << strerror(errno);
		return BIND_FAILED;
	}

	LOG(debug) << "bind done";

	if (listen(server_sock, backlog) < 0) {
		LOG(error) << "listen error";
		LOG(error) << strerror(errno);
		return LISTEN_FAILED;
	}
	LOG(debug) << "listen done";

	return server_sock;
}

std::string SocketTool::ReadMessage(int socketfd, bool& pipe_broken) {
	const int   READBUF_LEN = 8192;
	char	    readbuf[ READBUF_LEN ];
	int	    read_len;
	std::string msg = "";
	// while ((read_len = recv(socketfd, readbuf, READBUF_LEN, MSG_DONTWAIT)) > 0) {
	// 	readbuf[ read_len ] = '\0';
	// 	msg += std::string(readbuf, read_len);
	// 	// printf("readbuf:%s\n", readbuf);
	// }
	read_len = recv(socketfd, readbuf, READBUF_LEN, MSG_DONTWAIT);
	if (read_len == 0 || (read_len < 0 && errno != EAGAIN)) {
		pipe_broken = true;
		return "";
	}
	else if (read_len > 0) {
		readbuf[ read_len ] = '\0';
		msg += std::string(readbuf, read_len);
	}

	pipe_broken = false;
	return msg;
}

bool SocketTool::IsIpv4address(const std::string& address) {

	std::regex pattern("^(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9])"
			   "\\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-"
			   "9]"
			   "|0)\\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|["
			   "1-9]|0)\\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{"
			   "1}|[0-9])$");
	return std::regex_match(address, pattern) ? true : false;
}

std::string SocketTool::GetIpv4addressByDomainName(const std::string& domain_name) {
	struct hostent* host = gethostbyname(domain_name.c_str());
	if (!host || !( struct in_addr* )host->h_addr_list[ 0 ]) {
		LOG(error) << "domain_name illegal : " << domain_name;
		return "";
	}
	return std::string(inet_ntoa(*( struct in_addr* )host->h_addr_list[ 0 ]));
}

int SocketTool::ConnectToServer(std::string server_ip, uint16_t server_port, bool block) {
	int socketfd = CreateSocket(block);
	if (socketfd < 0) {
		LOG(error) << "create socket failed";
		return CREATE_SOCKET_FAILED;
	}

	if (!IsIpv4address(server_ip))
		server_ip = GetIpv4addressByDomainName(server_ip);
	if (server_ip.empty()) {
		LOG(error) << "server ip illegal";
		return ILLEGAL_ADDRESS;
	}

	struct sockaddr_in remote_addr;		       //服务器端网络地址结构体
	memset(&remote_addr, 0, sizeof(remote_addr));  //数据初始化--清零
	remote_addr.sin_family	    = AF_INET;	       //设置为IP通信
	remote_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());  //服务器IP地址
	remote_addr.sin_port	    = htons(server_port);	     //服务器端口号

	if (connect(socketfd, ( struct sockaddr* )&remote_addr, sizeof(struct sockaddr)) < 0) {
		LOG(error) << "connect to server " << server_ip << ":" << server_port << " failed";
		LOG(error) << strerror(errno);
		return CONNECT_ERROR;
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

SockAddress SocketTool::GetSockAddress(int socketfd) {
	struct sockaddr_in loc_addr;
	socklen_t	   len = 0;
	SockAddress	   sock_address;

	if (-1 == getsockname(socketfd, ( struct sockaddr* )&loc_addr, &len))
		return SockAddress();
	return ParseSockAddr(&loc_addr);
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
