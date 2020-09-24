#include "tcpclient.h"
#include "sockettool.h"
#include <assert.h>
#include <ev++.h>

using namespace nethelper;

TcpClient::TcpClient() {
	this->evloop_ = ev_loop_new(EVFLAG_AUTO);
	this->SetMessageArrivedCb(std::bind(&TcpClient::DefaultMessageArrivedCb, this,
					    std::placeholders::_1, std::placeholders::_2));
}
TcpClient::~TcpClient() {
	this->connection_->Disconnect();
	ev_break(this->evloop_);
	ev_loop_destroy(this->evloop_);
}

void TcpClient::Connect(const std::string& remote_ip, uint16_t remote_port) {

	int socketfd = SocketTool::ConnectToServer(remote_ip, remote_port, true);
	this->connection_.reset(new TcpConnection(socketfd, remote_ip, remote_port));
	connection_->receive_message_watcher().set< TcpClient, &TcpClient::MessageArrivedCb >(this);
	connection_->receive_message_watcher().set(this->evloop_);
	connection_->receive_message_watcher().start(connection_->connection_fd(), ev::READ);

	connection_->send_message_watcher().set< TcpClient, &TcpClient::MessageArrivedCb >(this);
	connection_->send_message_watcher().set(this->evloop_);

	LOG(info) << "connected to remote " << remote_ip << ":" << remote_port;
	ev_run(this->evloop_, 0);
}

void TcpClient::SetMessageArrivedCb(const MessageArrivedCallback& cb) {
	this->message_arrived_cb_ = cb;
}

/* private methods */
void TcpClient::MessageArrivedCb(ev::io& watcher, int revents) {
	LOG(info) << "new message from " << this->connection_->remote_ip() << ":"
		  << this->connection_->remote_port();

	std::string message = SocketTool::ReadMessage(watcher.fd);
	LOG(info) << message;
	if (message.empty())
		this->Reconnect();
	else
		this->message_arrived_cb_(*(this->connection_), message);
}

void TcpClient::Reconnect() {
	this->connection_->Disconnect();
	this->Connect(this->connection_->remote_ip(), this->connection_->remote_port());
}

bool TcpClient::SendMessage(const std::string& message, bool close_on_sent) {
	if (message.empty()) {
		if (close_on_sent)
			this->connection_->Disconnect();
		return true;
	}

	int sent_len = send(this->connection_->connection_fd(), message.c_str(), message.size(), 0);
	if (sent_len == static_cast< int >(message.size())) {
		if (close_on_sent)
			this->connection_->Disconnect();
		return true;
	}
	else if (sent_len == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
		this->Reconnect();
		LOG(error) << "send message failed : " << strerror(errno);
		LOG(error) << "reconnecting";
		return false;
	}
	else {
		std::string rest_of_message = sent_len > 0 ? message.substr(sent_len) : message;
		this->connection_->PushMessageIntoSendBuffer(rest_of_message, close_on_sent);

		if (!this->connection_->send_message_watcher().active) {
			LOG(debug) << "start write event watch";
			this->connection_->send_message_watcher().start(
				this->connection_->connection_fd(), ev::WRITE);
		}
		return true;
	}
}

void TcpClient::SendIoWatcherCb(ev::io& watcher, int revents) {
	TcpConnection* tcpconnection =
		reinterpret_cast< TcpConnectionContext* >(&watcher)->tcpconnection;
	SendBuffer& send_buffer	  = tcpconnection->send_buffer();
	int	    connection_fd = tcpconnection->connection_fd();
	size_t	    rest_len	  = send_buffer.buffer.size() - send_buffer.sent_offset;
	LOG(debug) << "send buffer : " << send_buffer.buffer;

	int sent_len = send(connection_fd, send_buffer.buffer.c_str() + send_buffer.sent_offset,
			    rest_len, 0);
	if (sent_len < 0) {
		if (sent_len != EAGAIN && sent_len != EWOULDBLOCK)
			this->Reconnect();
		return;
	}

	TcpConnectionContext* send_context = reinterpret_cast< TcpConnectionContext* >(&watcher);
	send_buffer.sent_offset += sent_len;
	if (send_buffer.IsEmpty()) {
		if (send_context->close_on_sent == true) {
			LOG(debug) << "send buffer empty, disconnect";
			this->connection_->Disconnect();
			return;
		}
		send_buffer.buffer.clear();
		send_buffer.sent_offset = 0;
		watcher.stop();
	}
}

void TcpClient::DefaultMessageArrivedCb(const TcpConnection& tcpconnection,
					const std::string&   msg) {
	LOG(debug) << "new message: " << msg;
	LOG(info) << "recerve message : " << msg;
	this->SendMessage("i am client , i have got your message!", false);
}
/* end of private methods */