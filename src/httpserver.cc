#include "httpserver.h"
#include "log.h"
#include <sstream>
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

	std::istringstream iss(message);
	std::string	strline;
	while (getline(iss, strline)) {
		if (strline == "\r") {
			std::string response = ResponseGet();
			send(client_fd, response.data(), response.size(), 0);
			LOG_INFO("sent response:\n%s\n", response.data());
		}
	}
}
std::string HttpServer::ResponseGet() {
	std::string ResponseBody   = MakeResponseBody("welcome to index!");
	std::string ResponseHeader = MakeResponseHeader(ResponseBody.size());
	return ResponseHeader + ResponseBody;
}
std::string HttpServer::MakeResponseHeader(int body_len) {
	std::string header = std::string("HTTP/1.1 200 OK\r\n")
			     + "Content-Type : text/html\r\n"
			     + "Content-Length:" + std::to_string(body_len)
			     + "\r\n" + "\r\n";
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
