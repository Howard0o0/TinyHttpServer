#include <ev++.h>
#include <tcpconnection.h>
#include <unistd.h>

/* public methods */

void TcpConnection::Disconnect() {
	close(this->connection_fd_);
	this->receive_message_watcher_.stop();
	this->send_message_watcher_.stop();
}
int TcpConnection::connection_fd() const {
	return this->connection_fd_;
}

ev::io& TcpConnection::receive_message_watcher() {
	return this->receive_message_watcher_;
}

ev::io& TcpConnection::send_message_watcher() {
	return this->send_message_watcher_;
}

/* end of public methods */
