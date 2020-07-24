#include "httpserver.h"
#include "httpcontext.h"
#include "log.h"
#include "threadpool.h"
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace nethelper;

/* public */

void HttpServer::StartLoop() {
	tcpserver_.SetOnMsgCallback(std::bind(&HttpServer::OnMsgArrived, this,
					      std::placeholders::_1,
					      std::placeholders::_2));
	tcpserver_.Start();
}

std::string HttpServer::FetchParamsStr(const std::string& message) {
	std::istringstream iss(message);
	std::string	   firstline;
	if (!getline(iss, firstline))
		return "";

	int startpos = firstline.find_first_of("/");
	int endpos   = firstline.find_first_of(" ", startpos);
	return firstline.substr(startpos + 1, endpos - startpos - 1);
}

void HttpServer::ResponseClient(int connfd, int statuscode,
				const std::string& body) {
	HttpContext httpcontext;
	httpcontext.SetStatuscode(statuscode);
	httpcontext.SetBody(body);
	httpcontext.SetHeader("content-type", "text/plain");
	std::string context = httpcontext.GetHttpContextStr();

	send(connfd, context.c_str(), context.size(), 0);
}
/* private */

void HttpServer::OnMsgArrived(int client_fd, const std::string& message) {

	std::istringstream iss(message);
	std::string	   strline;
	std::string	   response = ResponseGet();

	while (getline(iss, strline)) {
		if (strline.find("GET") != std::string::npos) {
			if (send(client_fd, response.data(), response.size(), 0)
			    == response.size())
				LOG_DEBUG("handled client(%d) \n", client_fd);
			else
				LOG_DEBUG("send was block\n");
			// LOG_INFO("sent response:\n%s\n", response.data());
		}
	}
	// close(client_fd);
}
std::string HttpServer::ResponseGet() {
	std::string ResponseBody   = MakeResponseBody("welcome to index!");
	std::string ResponseHeader = MakeResponseHeader(ResponseBody.size());
	return ResponseHeader + ResponseBody;
}
std::string HttpServer::MakeResponseHeader(int statuscode, int body_len) {
	std::string header = std::string("HTTP/1.1")
			     + std::to_string(statuscode) + " OK\r\n"
			     + "Content-Type : text/html\r\n";
	if (body_len > 0)
		header += "Content-Length:" + std::to_string(body_len) + "\r\n";
	header += "Connection: Keep-Alive\r\n";
	header += "\r\n";
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
