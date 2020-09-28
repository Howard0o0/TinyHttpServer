#ifndef TINYHTTPSERVER_SOCKETTOOL_H
#define TINYHTTPSERVER_SOCKETTOOL_H

#include <arpa/inet.h>
#include <fcntl.h>
#include <limits.h>
#include <log.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace nethelper {

enum ErrorCode {
	ILLEGAL_ADDRESS = INT_MIN,
	CREATE_SOCKET_FAILED,
	CONNECT_ERROR,
	CONVERT_IPADDR_TO_BINARY_FAILED,
	BIND_FAILED,
	LISTEN_FAILED
};

struct SockAddress {
	std::string ip;
	uint16_t    port;
	SockAddress(const std::string& _ip, uint16_t _port) : ip(_ip), port(_port) {
	}
	SockAddress() {
	}
};

class SocketTool {

    public:
	static int CreateListenSocket(int port, int backlog, bool block = false);
	static int ConnectToServer(std::string server_ip, uint16_t server_port, bool block = true);
	static std::string ReadMessage(int socketfd);
	static SockAddress ParseSockAddr(const struct sockaddr_in* sockaddr);

    private:
	static bool	   SetSocketReuse(int socket_fd);
	static void	   SetSocketNonblocking(int sockfd);
	static int	   CreateSocket(bool block = false);
	static bool	   IsIpv4address(const std::string& address);
	static std::string GetIpv4addressByDomainName(const std::string& domain_name);
};

}  // namespace nethelper
#endif