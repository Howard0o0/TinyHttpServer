#include "httpserver.h"
#include "log.h"

/* public */

void HttpServer::StartLoop() {
	tcpserver_.SetOnMsgCallback(std::bind(&HttpServer::OnMsgArrived,
					      std::placeholders::_1,
					      std::placeholders::_2));
	tcpserver_.Start();
}

/* private */

void HttpServer::OnMsgArrived(int client_fd, const std::string& message) {
	LOG_INFO("\n=====================");
	LOG_INFO("new message :\n%s \n", message.data());
	LOG_INFO("=====================\n");
	ResponseGet();
}
void HttpServer::ResponseGet() {
	std::string ResponseHeader = MakeResponseHeader();
}
std::string HttpServer::MakeResponseHeader() {
	std::string =
		"HTTP/1.0 200 OK\r\n" + " Content-Type : text/html\r\n " + "
}