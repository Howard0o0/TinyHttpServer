#ifndef SHADOWHTTP_SHADOWHTTPSERVER_H
#define SHADOWHTTP_SHADOWHTTPSERVER_H

#include "httpmessagecodec.h"
#include "tcprelay.h"

enum ResponseProxyclientMessageType { ESTABLISH, CONNECT_TIMEOUT };

class ShadowhttpServer : public TcpRelay {
    public:
    private:
	HttpMessageCodec codec_;

	virtual void ServerMessageArrivedCb(TcpConnection&     connection,
					    const std::string& message) override;
	virtual void ClientMessageArrivedCb(TcpConnection&     connection,
					    const std::string& message) override;

	void HandleHttpProxyMessage(TcpConnection& connection, const std::string& message);
	void ResponseProxyclient(enum ResponseProxyclientMessageType message_type);
};

#endif