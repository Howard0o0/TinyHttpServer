#include "tcprelay.h"
#include "sockettool.h"

void TcpRelay::Run(uint16_t listen_port) {
	this->StartTcpserver(listen_port);
}

/* pirvate methods */

void TcpRelay::StartTcpserver(uint16_t listen_port) {
	this->tcpserver_ = std::unique_ptr< TcpServer >(new TcpServer(listen_port, 1));
	this->tcpserver_->SetMessageArrivedCb(
		std::bind(&TcpRelay::ServerMessageArrivedCallbackPreprocess, this,
			  std::placeholders::_1, std::placeholders::_2));
	this->tcpserver_->SetConnectionReleaseCb(
		std::bind(&TcpRelay::TcpConnectionReleaseCb, this, std::placeholders::_1));
	this->tcpserver_->Start();
}

void TcpRelay::ServerMessageArrivedCallbackPreprocess(TcpConnection&	 connection,
						      const std::string& message) {
	this->ServerMessageArrivedCb(connection, message);
}

void TcpRelay::TcpConnectionReleaseCb(TcpConnection& connection) {

	LOG(debug) << "tunnel destroyed";
	std::lock_guard< std::mutex > guard(this->tunnels_lock_);
	this->tunnels.erase(connection.connection_fd());
	delete &connection;
	// this->tunnel_dict_[ connection.connection_fd() ].Destroy();
}

void TcpRelay::MessageArrivedCb(ev::io& watcher, int revents) {
	LOG(debug) << "tcp read io watch triggered";

	bool	       pipe_broken;
	std::string    message = SocketTool::ReadMessage(watcher.fd, pipe_broken);
	TcpConnection* connection =
		reinterpret_cast< TcpConnectionContext* >(&watcher)->tcpconnection;
	LOG(info) << "new message [len:" << message.size() << "] from (" << connection->remote_ip()
		  << ":" << connection->remote_port() << "):" << message;
	if (message.empty() && pipe_broken) {
		LOG(debug) << "read errno : " << errno;
		connection->Disconnect();
		watcher.stop();
		LOG(debug) << "disconnect";
		return;
	}

	this->MessageFromRemoteArrivedCb(*connection, message);
}
/* end of pirvate methods */

/* protected methods */

void TcpRelay::WatchConnection(TcpConnection* connection) {
	// std::lock_guard< std::mutex > guard(this->evloop_lock_);

	connection->receive_message_watcher().set< TcpRelay, &TcpRelay::MessageArrivedCb >(this);
	connection->receive_message_watcher().set(this->tcpserver_->GetEvloop());
	connection->receive_message_watcher().start(connection->connection_fd(), ev::READ);

	connection->SetDisconnectCb(
		std::bind(&TcpRelay::TcpConnectionReleaseCb, this, std::placeholders::_1));
	LOG(debug) << "watch connection success";
}
/* end of protected methods */