#ifndef MEN_MEN_H
#define MEN_MEN_H

#include "httpserver.h"
#include <string>

using namespace nethelper;

class Men : public HttpServer {
    public:
	Men(int port, int threadnum = 1) : HttpServer(port, threadnum) {
	}

    private:
	void OnMsgArrived(int client_fd, const std::string& message) override;
	std::string ScrawResult(const std::string& params);
};

#endif