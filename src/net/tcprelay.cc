#include "tcprelay.h"

TcpRelay::TcpRelay(uint16_t listen_port) {
	this->threadpool_.Start(2);
	this->threadpool_.RunTask(std::bind(&TcpRelay::StartTcpserver, this));
	this->threadpool_.RunTask(std::bind(&TcpRelay::StartTcpclient, this));
}

/* pirvate methods */

void TcpRelay::StartTcpserver() {
	this->tcpserver_ = std::unique_ptr< TcpServer >(new TcpServer(8080, 1));
	this->tcpserver_->SetMessageArrivedCb(std::bind(&TcpRelay::ServerMessageArrivedCb, this,
							std::placeholders::_1,
							std::placeholders::_2));
	this->tcpserver_->Start();
}
void TcpRelay::StartTcpclient() {
	this->tcpclient_ = std::unique_ptr< TcpClient >(new TcpClient);
}
/* end of pirvate methods */