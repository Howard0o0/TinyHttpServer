#ifndef TINYHTTPSERVER_SOCKETTOOL_H
#define TINYHTTPSERVER_SOCKETTOOL_H

#include <fcntl.h>
#include <log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace nethelper {

class SocketTool {

    public:
	static int	   CreateListenSocket(int port, int backlog,
					      bool block = false);
	static std::string ReadMessage(int socketfd);

    private:
	static bool SetSocketReuse(int socket_fd);
	static void SetSocketNonblocking(int sockfd);
};

}  // namespace nethelper
#endif