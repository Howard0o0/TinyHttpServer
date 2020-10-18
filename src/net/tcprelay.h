#ifndef SHADOWHTTP_TCPRELAY_H
#define SHADOWHTTP_TCPRELAY_H

#include "callback.h"
#include "tcpclient.h"
#include "tcpserver.h"
#include "threadpool.h"
#include "tunnel.h"
#include <memory>

class TcpRelay {
    public:
	void Run(uint16_t listen_port);

    private:
	void	     ServerMessageArrivedCallbackPreprocess(TcpConnection&     connection,
							    const std::string& message);
	virtual void ServerMessageArrivedCb(TcpConnection&     connection,
					    const std::string& message) = 0;
	virtual void ClientMessageArrivedCb(TcpConnection&     connection,
					    const std::string& message) = 0;

	void StartTcpserver(uint16_t listen_port);
	void StartTcpclient();
	void TcpConnectionReleaseCb(TcpConnection& connection);

    protected:
	std::unique_ptr< TcpServer >	  tcpserver_;
	std::unique_ptr< TcpClient >	  tcpclient_;
	std::unordered_map< int, Tunnel > tunnel_dict_;  // <fd,tunnel>
};

#endif