#include <ev++.h>
#include <tcpconnection.h>
#include <unistd.h>

/* public methods */

TcpConnection::~TcpConnection() {
	this->Disconnect();
}
void TcpConnection::Disconnect() {
	close(this->connection_fd_);
	this->receive_context_.io_watcher.stop();
	this->send_context_.io_watcher.stop();
}
int TcpConnection::connection_fd() const {
	return this->connection_fd_;
}

ev::io& TcpConnection::receive_message_watcher() {
	return this->receive_context_.io_watcher;
}

ev::io& TcpConnection::send_message_watcher() {
	return this->send_context_.io_watcher;
}

void TcpConnection::PushMessageIntoSendBuffer(const std::string& message, bool close_on_sent) {
	this->send_buffer_.buffer.append(message);
	this->send_context_.close_on_sent = close_on_sent;
}

SendBuffer& TcpConnection::send_buffer() {
	return this->send_buffer_;
}

std::string TcpConnection::remote_ip() const {
	return this->remote_ip_;
}
uint16_t TcpConnection::remote_port() const {
	return this->remote_port_;
}
/* end of public methods */
