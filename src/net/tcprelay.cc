#include "tcprelay.h"

void TcpRelay::Run(uint16_t listen_port) {
	this->StartTcpclient();
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
void TcpRelay::StartTcpclient() {
	this->tcpclient_ = std::unique_ptr< TcpClient >(new TcpClient);
	this->tcpclient_->SetMessageArrivedCb(std::bind(&TcpRelay::ClientMessageArrivedCb, this,
							std::placeholders::_1,
							std::placeholders::_2));
	this->tcpclient_->SetConnectionReleaseCb(
		std::bind(&TcpRelay::TcpConnectionReleaseCb, this, std::placeholders::_1));
}

void TcpRelay::ServerMessageArrivedCallbackPreprocess(TcpConnection&	 connection,
						      const std::string& message) {
	this->ServerMessageArrivedCb(connection, message);
}

void TcpRelay::TcpConnectionReleaseCb(TcpConnection& connection) {
	std::string connection_id =
		connection.remote_ip() + std::to_string(connection.remote_port())
		+ connection.local_ip() + std::to_string(connection.local_port());
	auto other_connection =
		this->tunnel_dict_[ connection_id ].connection_with_remote.get() == &connection
			? this->tunnel_dict_[ connection_id ].connection_with_client
			: this->tunnel_dict_[ connection_id ].connection_with_remote;
	std::string other_connection_id =
		other_connection->remote_ip() + std::to_string(other_connection->remote_port())
		+ other_connection->local_ip() + std::to_string(other_connection->local_port());

	LOG(debug) << "tunnel destroyed";
	this->tunnel_dict_[ connection_id ].Destroy();
	this->tunnel_dict_.erase(connection_id);
	this->tunnel_dict_.erase(other_connection_id);
}

/* end of pirvate methods */