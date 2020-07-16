#include "httpserver.h"
#include "log.h"
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

/* public */

void HttpServer::StartLoop() {
	tcpserver_.SetOnMsgCallback(std::bind(&HttpServer::OnMsgArrived, this,
					      std::placeholders::_1,
					      std::placeholders::_2));
	tcpserver_.Start();
}

/* private */

void HttpServer::OnMsgArrived(int client_fd, const std::string& message) {
	LOG_INFO("\n=====================");
	LOG_INFO("new message :\n%s \n", message.data());
	LOG_INFO("=====================\n");
	std::string response = ResponseGet();
	send(client_fd, response.data(), response.size(), 0);
	LOG_INFO("sent response:\n%s\n", response.data());
}
std::string HttpServer::ResponseGet() {
	std::string ResponseHeader = MakeResponseHeader();
	std::string ResponseBody   = MakeResponseBody("welcome to index!");
	return ResponseHeader + ResponseBody;
}
std::string HttpServer::MakeResponseHeader() {
	std::string header = std::string("HTTP/1.0 200 OK\r\n")
			     + "Content-Type : text/html\r\n" + "\r\n";
	return header;
}
std::string HttpServer::MakeResponseBody(const std::string& body) {
	std::string response_body =
		std::string("<HTML>\r\n")
		+ "<HEAD><TITLE>tiny http server</TITLE></HEAD>\r\n"
		+ "<BODY>\r\n" + "<h1>" + body + "</h1>\r\n" + "</BODY>\r\n"
		+ "</HTML>\r\n";
	return response_body;
}
