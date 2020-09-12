
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
	static int CreateListenSocket(int port, int backlog,
				      bool block = false);

    private:
	static void SetSocketNonblocking(int sockfd);
	static void SetSocketReuse(int socket_fd);
};

}  // namespace nethelper
#endif