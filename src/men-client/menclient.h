#ifndef MEN_MENCLIENT_H
#define MEN_MENCLIENT_H

#include "httpcontext.h"
#include "httprequest.h"

class MenClient {
    public:
	MenClient(const std::string& server_ip, int server_port)
		: server_ip_(server_ip), server_port_(server_port) {
	}
	std::string Query(const std::string& topic);

    private:
	std::string server_ip_;
	int	    server_port_;
};

#endif