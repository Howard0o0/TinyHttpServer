#ifndef TINYHTTPSERVER_TCPSERVER_H
#define TINYHTTPSERVER_TCPSERVER_H

#include "connection.h"
#include "epollinfo.h"
#include "lockfreeque.h"
#include "log.h"
#include "worker.h"
#include <functional>

using namespace tinythreadpool;

namespace ths {

class TcpServer {
    public:
	TcpServer(int port, int backlog = 2048)
		: port_(port), backlog_(backlog),
		  worker_(std::bind(&TcpServer::OnMsgArrived, this,
				    std::placeholders::_1,
				    std::placeholders::_2)) {
		InitialEpollinfo();
	}
	void Start();
	void SingleLoop();
	void SetOnMsgCallback(const OnMsgCallback& cb);

    private:
	int	   port_;
	int	   backlog_;
	int	   server_sockfd_;
	Worker	worker_;
	EpollInfo     epollinfo_;
	OnMsgCallback on_msg_cb_;

	int  CreateSocket();
	void StartWorkThreadsPool();
	void OnMsgArrived(int client_fd, const std::string& msg) {
		LOG_INFO("===========new message==========\n");
		LOG_INFO("%s\n", msg.data());
		LOG_INFO("================================\n");
	}
	void		   InitialEpollinfo();
	void		   RegisterEpoll(int fd);
	std::vector< int > GotEpollActiveFd();
};

}  // namespace ths

#endif