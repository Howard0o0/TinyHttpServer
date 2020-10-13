#ifndef TINYHTTPSERVER_TCPCONNECTION_H
#define TINYHTTPSERVER_TCPCONNECTION_H

#include "callback.h"
#include <ev++.h>
#include <memory>
#include <string>

struct SendBuffer {
	std::string buffer;
	size_t	    sent_offset;

	bool IsEmpty() {
		return this->sent_offset == this->buffer.size();
	}
};

class TcpConnection;

struct TcpConnectionContext {
	ev::io	       io_watcher;
	TcpConnection* tcpconnection;
	bool	       close_on_sent;
	TcpConnectionContext(TcpConnection* _tcpconnection)
		: tcpconnection(_tcpconnection), close_on_sent(false) {
	}
};

class TcpConnection {
    public:
	TcpConnection(int connection_fd)
		: receive_context_(this), send_context_(this), connection_fd_(connection_fd) {
	}
	TcpConnection(int connection_fd, std::string remote_ip, uint16_t remote_port)
		: receive_context_(this), send_context_(this), connection_fd_(connection_fd),
		  remote_ip_(remote_ip), remote_port_(remote_port) {
	}
	TcpConnection(int connection_fd, std::string remote_ip, uint16_t remote_port,
		      std::string local_ip, uint16_t local_port)
		: receive_context_(this), send_context_(this), connection_fd_(connection_fd),
		  remote_ip_(remote_ip), remote_port_(remote_port), local_ip_(local_ip),
		  local_port_(local_port) {
	}
	~TcpConnection();
	bool operator==(const TcpConnection& connection) {
		return this->connection_fd_ == connection.connection_fd()
		       && this->remote_ip_ == connection.remote_ip()
		       && this->remote_port_ == connection.remote_port()
		       && this->local_ip_ == connection.local_ip()
		       && this->local_port_ == connection.local_port();
	}

	void	    Disconnect();
	int	    connection_fd() const;
	ev::io&	    receive_message_watcher();
	ev::io&	    send_message_watcher();
	void	    PushMessageIntoSendBuffer(const std::string& message, bool close_on_sent);
	SendBuffer& send_buffer();
	std::string remote_ip() const;
	uint16_t    remote_port() const;
	std::string local_ip() const;
	uint16_t    local_port() const;
	void	    SetDisconnectCb(const TcpConnectionReleaseCallback& disconnect_cb);

    private:
	TcpConnectionContext	     receive_context_;
	TcpConnectionContext	     send_context_;
	SendBuffer		     send_buffer_;
	int			     connection_fd_;
	std::string		     remote_ip_;
	uint16_t		     remote_port_;
	std::string		     local_ip_;
	uint16_t		     local_port_;
	TcpConnectionReleaseCallback disconnect_cb_;
};

#endif