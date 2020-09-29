#ifndef SHADOWHTTP_TUNNEL_H
#define SHADOWHTTP_TUNNEL_H

#include "tcpconnection.h"

struct Tunnel {
	TcpConnection* connection_with_client;
	TcpConnection* connection_with_remote;

	Tunnel(TcpConnection* conn_with_client, TcpConnection* conn_with_remote)
		: connection_with_client(conn_with_client),
		  connection_with_remote(conn_with_remote) {
	}
	Tunnel() = default;

	void Destroy() {
		if (this->connection_with_client)
			delete (this->connection_with_client);
		if (this->connection_with_remote)
			delete (this->connection_with_remote);
	}
};

#endif