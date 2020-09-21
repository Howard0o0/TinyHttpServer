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
	TcpServer(int port, int threadnum = 4) : port_(port), io_thread_cnt_(threadnum) {
		this->SetMessageArrivedCb(std::bind(&TcpServer::DefaultMessageArrivedCb, this,
						    std::placeholders::_1, std::placeholders::_2));
	}
	void Start();
	void SetMessageArrivedCb(const MessageArrivedCallback& cb);
	bool SendMessage(TcpConnection* tcpconnection, const std::string& message,
			 bool close_on_sent);

    private:
	int		       port_;
	const int	       backlog_ = 10000;
	MessageArrivedCallback message_arrived_cb;

	ThreadPool io_threads_;
	int	   io_thread_cnt_;

	void SendIoWatcherCb(ev::io& watcher, int revents);
	void ConnectionArrivedCb(ev::io& watcher, int revents);
	void MessageArrivedCb(ev::io& watcher, int revents);
	void EventLoopThreadFunc(int port);

	void DefaultMessageArrivedCb(const TcpConnection& tcpconnection, const std::string& msg) {
		LOG_DEBUG("===========new message==========\n");
		LOG_DEBUG("%s\n", msg.data());
		LOG_DEBUG("================================\n");

		this->SendMessage(const_cast< TcpConnection* >(&tcpconnection), "hello!", false);
		this->SendMessage(const_cast< TcpConnection* >(&tcpconnection), "connection close!", true);

	}
};

}  // namespace nethelper

#endif