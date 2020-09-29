#include "log.h"
#include <ev++.h>
#include <tcpconnection.h>
#include <unistd.h>

/* public methods */

TcpConnection::~TcpConnection() {
	this->Disconnect();
}
void TcpConnection::Disconnect() {
	if (this->connection_fd_ == -1)
		return;
	close(this->connection_fd_);
	this->receive_context_.io_watcher.stop();
	this->send_context_.io_watcher.stop();
	this->connection_fd_ = -1;
	LOG(debug) << "disconnect with " << this->remote_ip_ << ":" << this->remote_port_;

	if (this->disconnect_cb_)
		this->disconnect_cb_();
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

void TcpConnection::SetDisconnectCb(Task disconnect_cb) {
	this->disconnect_cb_ = disconnect_cb;
}
/* end of public methods */
