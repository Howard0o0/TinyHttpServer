#include "shadowhttpserver.h"
#include "httpmessagecodec.h"
#include <thread>

#include "sockettool.h"

/* public methods */

ShadowhttpServer::ShadowhttpServer() {
	this->threadpool_.Start(10);
}

/* end of public methods */

/* private methods */

void ShadowhttpServer::ServerMessageArrivedCb(TcpConnection&	 connection,
					      const std::string& message) {
	LOG(debug) << "shadowhttp server receive [len:" << message.size() << "] :" << message;

	std::thread t1(std::bind(&ShadowhttpServer::HandleHttpProxyMessage, this,
				 std::ref(connection), *const_cast< std::string* >(&message)));
	t1.detach();
}
void ShadowhttpServer::MessageFromRemoteArrivedCb(TcpConnection&     connection,
						  const std::string& message) {

	std::lock_guard< std::mutex > guard(this->tunnels_lock_);

	if (!this->tunnels.count(connection.connection_fd())) {
		LOG(error) << "can't find tunnel ";
		return;
	}
	TcpConnection* conn_with_client =
		new TcpConnection(this->tunnels[ connection.connection_fd() ]);
	if (this->tcpserver_->SendMessage(conn_with_client, message))
		LOG(debug) << "foward to proxy client success : " << message;
}
void ShadowhttpServer::HandleHttpProxyMessage(TcpConnection& connection, std::string& message) {
	enum HttpProxyMessageType http_proxymessage_type =
		this->codec_.GetHttpProxyMessageType(message);

	if (http_proxymessage_type == CONNECT)
		LOG(debug) << "CONNECT";
	else if (http_proxymessage_type == HTTPFORWARD)
		LOG(debug) << "HTTPFORWARD";
	else
		LOG(debug) << "FORWARD";

	if (http_proxymessage_type == CONNECT || http_proxymessage_type == HTTPFORWARD) {
		/* build forward tunnel and store tunnel in dict */
		struct SockAddress remote_addr = this->codec_.ScratchRemoteAddress(message);
		if (remote_addr.ip.empty())
			return;

		int conn_with_remote_fd =
			SocketTool::ConnectToServer(remote_addr.ip, remote_addr.port, true);
		if (conn_with_remote_fd < 0) {
			LOG(error) << "connect to " << remote_addr.ip << ":" << remote_addr.port
				   << "failed";
			return;
		}
		SockAddress local_socket_address = SocketTool::GetSockAddress(conn_with_remote_fd);
		TcpConnection* connection_with_remote =
			new TcpConnection(conn_with_remote_fd, remote_addr.ip, remote_addr.port,
					  "127.0.0.1", local_socket_address.port);
		this->WatchConnection(connection_with_remote);

		Tunnel			      tunnel(&connection, connection_with_remote);
		std::lock_guard< std::mutex > guard(this->tunnels_lock_);
		this->tunnels.emplace(connection.connection_fd(),
				      connection_with_remote->connection_fd());
		this->tunnels.emplace(connection_with_remote->connection_fd(),
				      connection.connection_fd());
		this->tunnels_lock_.unlock();

		if (http_proxymessage_type == CONNECT) {
			this->ResponseProxyclient(&connection, ESTABLISH);
			return;
		}
	}

	if (http_proxymessage_type == HTTPFORWARD && this->codec_.RefactorUrlpath(message) == false)
		return;

	std::lock_guard< std::mutex > guard(this->tunnels_lock_);
	/* forward message to remote */
	if (!this->tunnels.count(connection.connection_fd())) {
		LOG(error) << "can't find tunnel";
		return;
	}
	TcpConnection* connection_with_remote =
		new TcpConnection(this->tunnels[ connection.connection_fd() ]);

	if (this->tcpserver_->SendMessage(connection_with_remote, message) == false) {
		LOG(error) << "send to " << connection_with_remote->remote_ip() << ":"
			   << connection_with_remote->remote_port() << "failed : " << message;
		return;
	}
	LOG(debug) << "forward data to remote success : " << message;
}

void ShadowhttpServer::ResponseProxyclient(TcpConnection*		       connection,
					   enum ResponseProxyclientMessageType message_type) {
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

	if (this->tcpserver_->SendMessage(connection, response_message) == false)
		LOG(error) << "send to " << connection->remote_ip() << ":"
			   << connection->remote_port() << "failed : " << response_message;
}
/* end of private methods */