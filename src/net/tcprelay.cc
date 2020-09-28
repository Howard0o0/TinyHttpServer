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
	this->tcpserver_->Start();
}
void TcpRelay::StartTcpclient() {
	this->tcpclient_ = std::unique_ptr< TcpClient >(new TcpClient);
	this->tcpclient_->SetMessageArrivedCb(std::bind(&TcpRelay::ClientMessageArrivedCb, this,
							std::placeholders::_1,
							std::placeholders::_2));
}

void TcpRelay::ServerMessageArrivedCallbackPreprocess(TcpConnection&	 connection,
						      const std::string& message) {
	this->connection_with_proxyclient_ = &connection;
	this->ServerMessageArrivedCb(connection, message);
}

/* end of pirvate methods */