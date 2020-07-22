#include "sockettool.h"

using namespace nethelper;

void SocketTool::SetSocketNonblocking(int sockfd) {
	int opts;

	opts = fcntl(sockfd, F_GETFL);
	if (opts < 0) {
		LOG_ERR("fcntl F_GETFL error :%s\n", strerror(errno));
		exit(1);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(sockfd, F_SETFL, opts) < 0) {
		LOG_ERR("fcntl F_SETFL error :%s\n", strerror(errno));
		exit(1);
	}
}