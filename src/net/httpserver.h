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
	std::string ResponseGet();
	std::string MakeResponseHeader(int body_len = 0);
	std::string MakeResponseBody(const std::string& body);
};

#endif