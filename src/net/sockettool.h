#ifndef TINYHTTPSERVER_SOCKETTOOL_H
#define TINYHTTPSERVER_SOCKETTOOL_H

#include <fcntl.h>
#include <log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

namespace nethelper {

class SocketTool {

    public:
	static void SetSocketNonblocking(int sockfd);
};

}  // namespace nethelper
#endif