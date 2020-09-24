#ifndef TINYHTTPSERVER_SOCKETTOOL_H
#define TINYHTTPSERVER_SOCKETTOOL_H

#include <arpa/inet.h>
#include <fcntl.h>
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

struct SockAddress {
	std::string ip;
	uint16_t    port;
};

class SocketTool {

    public:
	static int	   CreateListenSocket(int port, int backlog, bool block = false);
	static int	   ConnectToServer(const std::string& server_ip, uint16_t server_port,
					   bool block = true);
	static std::string ReadMessage(int socketfd);
	static SockAddress ParseSockAddr(const struct sockaddr_in* sockaddr);

    private:
	static bool SetSocketReuse(int socket_fd);
	static void SetSocketNonblocking(int sockfd);
	static int  CreateSocket(bool block = false);
};

}  // namespace nethelper
#endif