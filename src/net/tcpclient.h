#ifndef SHADOWHTTP_TCPCLIENT_H
#define SHADOWHTTP_TCPCLIENT_H

#include "callback.h"
#include "tcpconnection.h"
#include "threadpool.h"
#include <ev++.h>
#include <memory>
#include <stdint.h>
#include <string>

using namespace nethelper;

class TcpClient {
    public:
	TcpClient();
	~TcpClient();
	TcpConnection* Connect(const std::string& remote_ip, uint16_t remote_port);
	void	       SetMessageArrivedCb(const MessageArrivedCallback& cb);
	void	       SetConnectionReleaseCb(const TcpConnectionReleaseCallback& cb);
	bool	       SendMessage(TcpConnection* connection, const std::string& message,
				   bool close_on_sent = false);
	void	       WatchConnection(TcpConnection* connection);

    private:
	MessageArrivedCallback	     message_arrived_cb_;
	TcpConnectionReleaseCallback connection_release_cb_;
	struct ev_loop*		     evloop_;
	bool			     loop_run_;
	ThreadPool		     threadpool_;
	std::mutex		     evloop_lock_;

	void MessageArrivedCb(ev::io& watcher, int revents);
	void SendIoWatcherCb(ev::io& watcher, int revents);
	void StartLoop();

	void DefaultMessageArrivedCb(const TcpConnection& tcpconnection, const std::string& msg);
};

#endif