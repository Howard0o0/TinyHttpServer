#ifndef TINYHTTPSERVER_TCPCONNECTION_H
#define TINYHTTPSERVER_TCPCONNECTION_H

#include <ev++.h>
#include <string>

struct SendBuffer {
	std::string buffer;
	size_t	    sent_offset;

	bool IsEmpty() {
		return sent_offset == buffer.size();
	}
};

class TcpConnection {
    public:
	TcpConnection(int connection_fd) : connection_fd_(connection_fd) {
	}
	void	Disconnect();
	int	connection_fd() const;
	ev::io& receive_message_watcher();
	ev::io& send_message_watcher();

    private:
	ev::io	   receive_message_watcher_;
	ev::io	   send_message_watcher_;
	int	   connection_fd_;
	SendBuffer sendbuffer_;
};

#endif