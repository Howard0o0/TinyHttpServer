#ifndef TINYHTTPSERVER_CONNECTION_H
#define TINYHTTPSERVER_CONNECTION_H

#include <netinet/in.h>
#include <sys/socket.h>

namespace ths {

class Connection {

    public:
	Connection(struct sockaddr_in client_addr, int client_sockfd) {
		client_addr_   = client_addr;
		client_sockfd_ = client_sockfd;
	}

    private:
	struct sockaddr_in client_addr_;
	int		   client_sockfd_;
};

}  // namespace ths

#endif