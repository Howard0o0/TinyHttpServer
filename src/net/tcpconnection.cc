#include "log.h"
#include <ev++.h>
#include <tcpconnection.h>
#include <unistd.h>

/* public methods */

TcpConnection::~TcpConnection() {
	// this->Disconnect();
	LOG(debug) << "tcpconnection destructor called";
}
void TcpConnection::Disconnect() {
	if (this->connection_fd_ == -1)
		return;
	close(this->connection_fd_);
	this->connection_fd_ = -1;
	this->receive_message_watcher().stop();
	this->send_message_watcher().stop();

	if (this->disconnect_cb_)
		this->disconnect_cb_(*this);
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

std::string TcpConnection::local_ip() const {
	return this->local_ip_;
}
uint16_t TcpConnection::local_port() const {
	return this->local_port_;
}

void TcpConnection::SetDisconnectCb(const TcpConnectionReleaseCallback& disconnect_cb) {
	this->disconnect_cb_ = disconnect_cb;
}
/* end of public methods */
