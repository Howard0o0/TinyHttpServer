#ifndef TINYHTTPSERVER_TCPSERVER_H
#define TINYHTTPSERVER_TCPSERVER_H

#include "log.h"
#include "threadpool.h"
#include <ev++.h>
#include <functional>
#include <unordered_map>

using namespace nethelper;

namespace nethelper {

class TcpServer {
    public:
	TcpServer(int port, int threadnum = 4)
		: port_(port), io_thread_cnt_(threadnum) {
		this->SetMessageArrivedCb(std::bind(
			&TcpServer::DefaultMessageArrivedCb, this,
			std::placeholders::_1, std::placeholders::_2));
	}
	void Start();
	void SetMessageArrivedCb(const MessageArrivedCallback& cb);

    private:
	int	      port_;
	const int     backlog_ = 10000;
	int	      server_sockfd_;
	MessageArrivedCallback message_arrived_cb;

	ThreadPool io_threads_;
	int	   io_thread_cnt_;

	struct ev_loop* GetEvloop(int listenfd);
	void		ConnectionArrivedCb(ev::io& watcher, int revents);
	void		MessageArrivedCb(ev::io& watcher, int revents);
	void		AddEventIntoEvloop(int fd, int event,
					   const EviowatchCb& ev_io_watch_cb,
					   struct ev_loop*    evloop);
	void		AddEventIntoEvloop2(int fd, int event, int watcher_type,
					    struct ev_loop* evloop);
	void		EventLoopThreadFunc(int port);
	void DefaultMessageArrivedCb(const TcpConnection& tcpconnection,
				     const std::string&	  msg) {

		LOG_DEBUG("===========new message==========\n");
		LOG_DEBUG("%s\n", msg.data());
		LOG_DEBUG("================================\n");
	}
};

}  // namespace nethelper

#endif