#include "shadowhttpserver.h"
#include "httpmessagecodec.h"
#include "sockettool.h"

/* private methods */

void ShadowhttpServer::ServerMessageArrivedCb(TcpConnection&	 connection,
					      const std::string& message) {
	LOG(debug) << "shadowhttp server receive:" << message;
	this->HandleHttpProxyMessage(connection, message);
}
void ShadowhttpServer::ClientMessageArrivedCb(TcpConnection&	 connection,
					      const std::string& message) {
	this->tcpserver_->SendMessage(this->connection_with_proxyclient_, message);
}
void ShadowhttpServer::HandleHttpProxyMessage(TcpConnection&	 connection,
					      const std::string& message) {
	enum HttpProxyMessageType http_proxymessage_type =
		this->codec_.GetHttpProxyMessageType(message);

	if (http_proxymessage_type == HttpProxyMessageType::CONNECT) {
		struct SockAddress remote_addr = this->codec_.ScratchRemoteAddress(message);
		LOG(debug) << "handle CONNECT " << remote_addr.ip << ":" << remote_addr.port;
		if (this->tcpclient_->Connect(remote_addr.ip, remote_addr.port) == true) {
			LOG(debug) << "tunnel with remote established";
			this->ResponseProxyclient(ESTABLISH);
		}
		else {
			LOG(debug) << "tunnel with remote build failed";
			this->ResponseProxyclient(CONNECT_TIMEOUT);
		}
	}
	else if (http_proxymessage_type == HttpProxyMessageType::HTTPFORWARD) {
		LOG(debug) << "handle HTTPFORWARD";
		struct SockAddress remote_addr = this->codec_.ScratchRemoteAddress(message);
		if (this->tcpclient_->Connect(remote_addr.ip, remote_addr.port) == false) {
			LOG(error) << "http forward connect to " << remote_addr.ip << ":"
				   << remote_addr.port << " failed";
			return;
		}
		std::string refactored_message = message;
		if (this->codec_.RefactorUrlpath(refactored_message) == false)
			return;
		this->tcpclient_->SendMessage(refactored_message);
	}
	else {

		LOG(debug) << "handle FORWARD";
		// FORWARD
		if (!this->tcpclient_->is_connected())
			return;
		this->tcpclient_->SendMessage(message);
	}
}

void ShadowhttpServer::ResponseProxyclient(enum ResponseProxyclientMessageType message_type) {
	std::string response_message;
	switch (message_type) {
	case ESTABLISH:
		response_message = "HTTP/1.1 200 Connection Established\r\n\r\n";
		break;
	case CONNECT_TIMEOUT:
		response_message = "HTTP/1.1 504 Gateway Time-out\r\n\r\n";
		break;
	default:
		LOG(error) << "message_type illegal";
		return;
	}

	this->tcpserver_->SendMessage(this->connection_with_proxyclient_, response_message);
}
/* end of private methods */