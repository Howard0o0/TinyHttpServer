#ifndef TINYHTTPSERVER_TCPSERVER_H
#define TINYHTTPSERVER_TCPSERVER_H

#include "epolltool.h"
#include "lockfreeque.h"
#include "log.h"
#include "worker.h"
#include <functional>

using namespace nethelper;

namespace nethelper {

class TcpServer {
    public:
	TcpServer(int port, int backlog = 1024)
		: port_(port), backlog_(backlog),
		  worker_(std::bind(&TcpServer::OnMsgArrived, this,
				    std::placeholders::_1,
				    std::placeholders::_2)) {
	}
	void Start();
	void SingleLoop();
	void SetOnMsgCallback(const OnMsgCallback& cb);

    private:
	int	   port_;
	int	   backlog_;
	int	   server_sockfd_;
	Worker	worker_;
	OnMsgCallback on_msg_cb_;

	int  CreateSocket(bool block = true);
	void StartWorkThreadsPool();
	void OnMsgArrived(int client_fd, const std::string& msg) {
		LOG_DEBUG("===========new message==========\n");
		LOG_DEBUG("%s\n", msg.data());
		LOG_DEBUG("================================\n");
	}

	void SetSocketReuse(int socket_fd);
};

}  // namespace nethelper

#endif