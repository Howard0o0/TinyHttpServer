#include "tcpclient.h"
#include "sockettool.h"
#include "threadpool.h"
#include <assert.h>
#include <ev++.h>
#include <functional>
#include <thread>

using namespace nethelper;

TcpClient::TcpClient() : loop_run_(false) {
	this->evloop_ = ev_loop_new(EVFLAG_AUTO);
	this->SetMessageArrivedCb(std::bind(&TcpClient::DefaultMessageArrivedCb, this,
					    std::placeholders::_1, std::placeholders::_2));
	if (!this->loop_run_) {
		this->threadpool_.Start(1);
		this->threadpool_.RunTask(std::bind(&TcpClient::StartLoop, this));
		LOG(debug) << "run loop";
		while (!this->loop_run_) {
			LOG(debug) << "waiting loop run";
			sleep(1);
		}
	}
}
TcpClient::~TcpClient() {
	// ev_break(this->evloop_, EVBREAK_ONE);
	// ev_loop_destroy(this->evloop_);
}

static void null_listen_cb(ev::timer& w, int revents) {
	LOG(debug) << "receive a new connection";
	// w.start(1., 0.);
	// accept(w.fd, NULL, NULL);
	// close(w.fd);

	return;
}

void TcpClient::StartLoop() {
	LOG(debug) << "start loop";
	if (!this->evloop_) {
		LOG(error) << "evloop is null";
		return;
	}

	int null_listenfd = SocketTool::CreateListenSocket(10050, 10000, false);

	ev::timer timer_watcher;
	timer_watcher.set< null_listen_cb >();
	timer_watcher.set(this->evloop_);
	timer_watcher.start(0., 3.);
	this->loop_run_ = true;
	ev_run(this->evloop_, 0);
}

// bool TcpClient::is_connected() const {
// 	return this->is_connected_;
// }

void TcpClient::WatchConnection(TcpConnection* connection) {
	// std::lock_guard< std::mutex > guard(this->evloop_lock_);

	connection->receive_message_watcher().set< TcpClient, &TcpClient::MessageArrivedCb >(this);
	connection->receive_message_watcher().set(this->evloop_);
	connection->receive_message_watcher().start(connection->connection_fd(), ev::READ);

	connection->send_message_watcher().set< TcpClient, &TcpClient::SendIoWatcherCb >(this);
	connection->send_message_watcher().set(this->evloop_);
	connection->send_message_watcher().stop();

	if (this->connection_release_cb_)
		connection->SetDisconnectCb(this->connection_release_cb_);
	LOG(debug) << "watch connection success";
}

TcpConnection* TcpClient::Connect(const std::string& remote_ip, uint16_t remote_port) {

	int socketfd = SocketTool::ConnectToServer(remote_ip, remote_port, true);
	if (socketfd < 0)
		return nullptr;

	SockAddress local_socket_address = SocketTool::GetSockAddress(socketfd);
	local_socket_address.ip		 = "127.0.0.1";
	TcpConnection* connection =
		new TcpConnection(socketfd, remote_ip, remote_port, local_socket_address.ip,
				  local_socket_address.port);
	this->WatchConnection(connection);
	LOG(info) << "connected to remote " << remote_ip << ":" << remote_port;
	return connection;
}

void TcpClient::SetMessageArrivedCb(const MessageArrivedCallback& cb) {
	this->message_arrived_cb_ = cb;
}

bool TcpClient::SendMessage(TcpConnection* connection, const std::string& message,
			    bool close_on_sent) {
	if (message.empty() || connection->connection_fd() <= 0) {
		if (close_on_sent) {
			LOG(debug) << "message to send empty, closed";
			connection->Disconnect();
		}
		return true;
	}

	LOG(debug) << "sending message [len:" << message.size() << "] to "
		   << connection->remote_ip() << ":" << connection->remote_port() << " : "
		   << message;

	int sent_len =
		send(connection->connection_fd(), message.c_str(), message.size(), MSG_DONTWAIT);
	if (sent_len == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
		LOG(error) << "send message failed : " << strerror(errno);
		connection->Disconnect();
		return false;
	}

	// connection->receive_message_watcher().start();

	if (sent_len == static_cast< int >(message.size())) {
		LOG(debug) << "send done";
		if (close_on_sent)
			connection->Disconnect();
	}
	else {
		LOG(debug) << "message too large, register watcher to send left";
		std::string rest_of_message = sent_len > 0 ? message.substr(sent_len) : message;
		connection->PushMessageIntoSendBuffer(rest_of_message, close_on_sent);

		if (!connection->send_message_watcher().active) {
			LOG(debug) << "start write event watch";
			connection->send_message_watcher().start(connection->connection_fd(),
								 ev::WRITE);
		}
	}

	return true;
}

void TcpClient::SetConnectionReleaseCb(const TcpConnectionReleaseCallback& cb) {
	this->connection_release_cb_ = cb;
}

/* private methods */

void TcpClient::MessageArrivedCb(ev::io& watcher, int revents) {
	// watcher.stop();
	LOG(debug) << "tcp read io watch triggered";

	/* 向上转换connection有误 */
	bool	       pipe_broken;
	std::string    message = SocketTool::ReadMessage(watcher.fd, pipe_broken);
	TcpConnection* connection =
		reinterpret_cast< TcpConnectionContext* >(&watcher)->tcpconnection;
	LOG(info) << "new message [len:" << message.size() << "] from (" << connection->remote_ip()
		  << ":" << connection->remote_port() << "):" << message;
	if (message.empty() && pipe_broken) {
		LOG(debug) << "read errno : " << errno;
		connection->Disconnect();
		LOG(debug) << "disconnect";
		return;
	}
	else if (this->message_arrived_cb_ && !message.empty()) {
		this->message_arrived_cb_(*(connection), message);
	}
	// watcher.start(watcher.fd, EV_READ);
}

// void TcpClient::Reconnect() {
// 	connection->Disconnect();
// 	this->Connect(connection->remote_ip(), connection->remote_port());
// }

void TcpClient::SendIoWatcherCb(ev::io& watcher, int revents) {
	TcpConnection* tcpconnection =
		reinterpret_cast< TcpConnectionContext* >(&watcher)->tcpconnection;
	SendBuffer& send_buffer	  = tcpconnection->send_buffer();
	int	    connection_fd = tcpconnection->connection_fd();
	size_t	    rest_len	  = send_buffer.buffer.size() - send_buffer.sent_offset;
	LOG(debug) << "send buffer : " << send_buffer.buffer;

	int sent_len = send(connection_fd, send_buffer.buffer.c_str() + send_buffer.sent_offset,
			    rest_len, MSG_DONTWAIT);
	if (sent_len < 0) {
		// if (sent_len != EAGAIN && sent_len != EWOULDBLOCK)
		// 	this->Reconnect();
		return;
	}

	TcpConnectionContext* send_context = reinterpret_cast< TcpConnectionContext* >(&watcher);
	send_buffer.sent_offset += sent_len;
	if (send_buffer.IsEmpty()) {
		if (send_context->close_on_sent == true) {
			LOG(debug) << "send buffer empty, disconnect";
			tcpconnection->Disconnect();
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
	this->SendMessage(const_cast< TcpConnection* >(&tcpconnection),
			  "i am client , i have got your message!", false);
}
/* end of private methods */