#ifndef TINYHTTPSERVER_TCPCONNECTION_H
#define TINYHTTPSERVER_TCPCONNECTION_H

#include <ev++.h>
#include <string>

class TcpConnection {
    public:
	TcpConnection(ev::io& io_watcher) : io_watcher_(&io_watcher){};
	void Disconnect();

    private:
	ev::io* io_watcher_;
};

#endif