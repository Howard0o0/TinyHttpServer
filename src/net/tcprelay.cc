#include "tcprelay.h"

void TcpRelay::Run(uint16_t listen_port) {
	this->StartTcpclient();
	while (!this->tcpclient_->is_connected())
		;
	this->StartTcpserver(listen_port);
}
/* pirvate methods */

void TcpRelay::StartTcpserver(uint16_t listen_port) {
	this->tcpserver_ = std::unique_ptr< TcpServer >(new TcpServer(8080, 1));
	this->tcpserver_->SetMessageArrivedCb(std::bind(&TcpRelay::ServerMessageArrivedCb, this,
							std::placeholders::_1,
							std::placeholders::_2));
	this->tcpserver_->Start();
}
void TcpRelay::StartTcpclient() {
	this->tcpclient_ = std::unique_ptr< TcpClient >(new TcpClient);
	this->tcpclient_->SetMessageArrivedCb(std::bind(&TcpRelay::ClientMessageArrivedCb, this,
							std::placeholders::_1,
							std::placeholders::_2));
}
/* end of pirvate methods */