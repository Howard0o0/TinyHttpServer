#ifndef SHADOWHTTP_TCPRELAY_H
#define SHADOWHTTP_TCPRELAY_H

#include "callback.h"
#include "tcpclient.h"
#include "tcpserver.h"
#include "threadpool.h"
#include "tunnel.h"
#include <memory>
#include <mutex>

class TcpRelay {
    public:
	void Run(uint16_t listen_port);

    private:
	void	     ServerMessageArrivedCallbackPreprocess(TcpConnection&     connection,
							    const std::string& message);
	virtual void ServerMessageArrivedCb(TcpConnection&     connection,
					    const std::string& message)	    = 0;
	virtual void MessageFromRemoteArrivedCb(TcpConnection&	   connection,
						const std::string& message) = 0;

	void StartTcpserver(uint16_t listen_port);
	void TcpConnectionReleaseCb(TcpConnection& connection);
	void MessageArrivedCb(ev::io& watcher, int revents);

    protected:
	std::unique_ptr< TcpServer >   tcpserver_;
	std::mutex		       tunnels_lock_;
	std::unordered_map< int, int > tunnels;	 // <fd,fd>

	void WatchConnection(TcpConnection* connection);
};

#endif