#ifndef SHADOWHTTP_SHADOWHTTPSERVER_H
#define SHADOWHTTP_SHADOWHTTPSERVER_H

#include "httpmessagecodec.h"
#include "tcprelay.h"
#include "tunnel.h"
#include <unordered_map>

enum ResponseProxyclientMessageType { ESTABLISH, CONNECT_TIMEOUT };

class ShadowhttpServer : public TcpRelay {
    public:
    private:
	HttpMessageCodec			  codec_;
	std::unordered_map< std::string, Tunnel > tunnel_dict_;

	virtual void ServerMessageArrivedCb(TcpConnection&     connection,
					    const std::string& message) override;
	virtual void ClientMessageArrivedCb(TcpConnection&     connection,
					    const std::string& message) override;

	void HandleHttpProxyMessage(TcpConnection& connection, std::string& message);
	void ResponseProxyclient(TcpConnection*			     connection,
				 enum ResponseProxyclientMessageType message_type);
};

#endif