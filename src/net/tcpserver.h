#ifndef TINYHTTPSERVER_TCPSERVER_H
#define TINYHTTPSERVER_TCPSERVER_H

#include "callback.h"
#include "log.h"
#include "tcpconnection.h"
#include "threadpool.h"
#include <ev++.h>
#include <functional>
#include <unordered_map>

using namespace nethelper;

namespace nethelper {

class TcpServer {
    public:
	TcpServer(int port, int threadnum = 4);
	~TcpServer();
	void Start();
	void SetMessageArrivedCb(const MessageArrivedCallback& cb);
	void SetConnectionReleaseCb(const TcpConnectionReleaseCallback& cb);
	bool SendMessage(TcpConnection* tcpconnection, const std::string& message,
			 bool close_on_sent = false);

    private:
	int			     port_;
	const int		     backlog_ = 10000;
	MessageArrivedCallback	     message_arrived_cb_;
	TcpConnectionReleaseCallback connection_release_cb_;

	ThreadPool io_threads_;
	int	   io_thread_cnt_;

	void SendIoWatcherCb(ev::io& watcher, int revents);
	void ConnectionArrivedCb(ev::io& watcher, int revents);
	void MessageArrivedCb(ev::io& watcher, int revents);
	void EventLoopThreadFunc(int port);

	void DefaultMessageArrivedCb(const TcpConnection& tcpconnection, const std::string& msg);
};

}  // namespace nethelper

#endif