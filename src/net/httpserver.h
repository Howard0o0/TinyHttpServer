#ifndef TINYHTTPSERVER_HTTPSERVER_H
#define TINYHTTPSERVER_HTTPSERVER_H

#include "tcpserver.h"

namespace nethelper {
class HttpServer {
    public:
	HttpServer(int port, int threadnum = 1) : tcpserver_(port, threadnum) {
	}
	virtual ~HttpServer() {
	}
	void StartLoop();

	std::string FetchParamsStr(const std::string& message);
	void	    ResponseClient(int connfd, int statuscode,
				   const std::string& body = "");
	void	    SendFile(int connfd, const std::string& filename);

    private:
	TcpServer tcpserver_;

	virtual void OnMsgArrived(TcpConnection&	     tcpconnection,
				  const std::string& message);
	std::string  ResponseGet();
	std::string  MakeResponseHeader(int statuscode, int body_len = 0);
	std::string  MakeResponseBody(const std::string& body);
};

}  // namespace nethelper
#endif