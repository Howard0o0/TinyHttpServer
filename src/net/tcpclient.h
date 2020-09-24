#ifndef SHADOWHTTP_TCPCLIENT_H
#define SHADOWHTTP_TCPCLIENT_H

#include "callback.h"
#include "tcpconnection.h"
#include <ev++.h>
#include <memory>
#include <stdint.h>
#include <string>

class TcpClient {
    public:
	TcpClient();
	~TcpClient();
	void Connect(const std::string& remote_ip, uint16_t remote_port);
	void SetMessageArrivedCb(const MessageArrivedCallback& cb);
	bool SendMessage(const std::string& message, bool close_on_sent = false);

    private:
	std::string remote_ip_;
	uint16_t    remote_port_;
	std::unique_ptr< TcpConnection > connection_;
	MessageArrivedCallback message_arrived_cb_;
	struct ev_loop*	       evloop_;

	void MessageArrivedCb(ev::io& watcher, int revents);
	void Reconnect();
	void SendIoWatcherCb(ev::io& watcher, int revents);

	void DefaultMessageArrivedCb(const TcpConnection& tcpconnection, const std::string& msg);
};

#endif