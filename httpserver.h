#ifndef TINYHTTPSERVER_HTTPSERVER_H
#define TINYHTTPSERVER_HTTPSERVER_H

#include "tcpserver.h"

using namespace ths;

class HttpServer {
    public:
	HttpServer(int port) : tcpserver_(port) {
	}
	void StartLoop();

    private:
	TcpServer tcpserver_;

	void	OnMsgArrived(int client_fd, const std::string& message);
	void	ResponseGet();
	std::string MakeResponseHeader();
}

#endif