#include "tcpserver.h"
#include "callback.h"
#include "lockfreethreadpool.h"
#include "log.h"
#include "sockettool.h"
#include <arpa/inet.h>
#include <ev++.h>
#include <ev.h>
#include <fcntl.h>
#include <memory>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace nethelper;

/* public */

TcpServer::TcpServer(int port, int threadnum) : port_(port), io_thread_cnt_(threadnum) {
	this->SetMessageArrivedCb(std::bind(&TcpServer::DefaultMessageArrivedCb, this,
					    std::placeholders::_1, std::placeholders::_2));
}
TcpServer::~TcpServer() {
}

void TcpServer::Start() {
	io_threads_.Start(io_thread_cnt_);
	for (int i = 0; i < io_thread_cnt_; ++i)
		io_threads_.RunTask(std::bind(&TcpServer::EventLoopThreadFunc, this, port_));
}

void TcpServer::SetMessageArrivedCb(const MessageArrivedCallback& cb) {
	message_arrived_cb_ = cb;
}

bool TcpServer::SendMessage(TcpConnection* connection, const std::string& message,
			    bool close_on_sent) {
	if (message.empty()) {
		if (close_on_sent)
			connection->Disconnect();
		;
		return true;
	}

	LOG(debug) << "sending message [len:" << message.size() << "] to ("
		   << connection->remote_ip() << ":" << connection->remote_port()
		   << ") : " << message;

	int sent_len =
		send(connection->connection_fd(), message.c_str(), message.size(), MSG_DONTWAIT);
	if (sent_len == static_cast< int >(message.size())) {
		if (close_on_sent)
			connection->Disconnect();
		;
		LOG(debug) << "send done";
		return true;
	}
	else if (sent_len == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
		connection->Disconnect();
		LOG(debug) << "send failed";
		return false;
	}
	else {
		std::string rest_of_message = sent_len > 0 ? message.substr(sent_len) : message;
		LOG(debug) << "message to send is too large, register io wathcer to send left : "
			   << rest_of_message.size();
		connection->PushMessageIntoSendBuffer(rest_of_message, close_on_sent);
		if (!connection->send_message_watcher().active) {
			LOG(debug) << "start write event watch";
			connection->send_message_watcher().start(connection->connection_fd(),
								 ev::WRITE);
		}
		return true;
	}
}

void TcpServer::SetConnectionReleaseCb(const TcpConnectionReleaseCallback& cb) {
	this->connection_release_cb_ = cb;
}
/* private */

void TcpServer::SendIoWatcherCb(ev::io& watcher, int revents) {
	TcpConnection* connection =
		reinterpret_cast< TcpConnectionContext* >(&watcher)->tcpconnection;
	SendBuffer* send_buffer	  = &connection->send_buffer();
	int	    connection_fd = connection->connection_fd();
	size_t	    rest_len	  = send_buffer->buffer.size() - send_buffer->sent_offset;
	LOG(debug) << "send buffer [len:" << send_buffer->buffer.size() << "] : " << rest_len;

	int sent_len = send(connection_fd, send_buffer->buffer.c_str() + send_buffer->sent_offset,
			    rest_len, MSG_DONTWAIT);
	if (sent_len < 0) {
		if (sent_len != EAGAIN && sent_len != EWOULDBLOCK)
			connection->Disconnect();
		;
		LOG(error) << "send failed : " << strerror(errno);
		return;
	}
	LOG(debug) << "sent " << sent_len << " bytes";

	TcpConnectionContext* send_context = reinterpret_cast< TcpConnectionContext* >(&watcher);
	send_buffer->sent_offset += sent_len;
	if (send_buffer->IsEmpty()) {
		if (send_context->close_on_sent == true) {
			LOG(debug) << "send buffer empty, disconnect";
			connection->Disconnect();
			;
			return;
		}
		send_buffer->buffer.clear();
		send_buffer->sent_offset = 0;
		watcher.stop();
	}
}

void TcpServer::MessageArrivedCb(ev::io& watcher, int revents) {

	TcpConnection* connection =
		reinterpret_cast< TcpConnectionContext* >(&watcher)->tcpconnection;
	bool	    pipe_broken;
	std::string message = SocketTool::ReadMessage(watcher.fd, pipe_broken);
	if (message.empty() && pipe_broken) {
		connection->Disconnect();
		LOG(debug) << "disconnect";
	}
	else
		this->message_arrived_cb_(*connection, message);
}

void TcpServer::ConnectionArrivedCb(ev::io& watcher, int revents) {

	int		   connfd = -1;
	struct sockaddr_in client_addr;
	socklen_t	   sockaddr_len = sizeof(client_addr);

	while ((connfd = accept4(watcher.fd, reinterpret_cast< struct sockaddr* >(&client_addr),
				 &sockaddr_len, SOCK_NONBLOCK))
	       > 0) {
		SockAddress sockaddr = SocketTool::ParseSockAddr(&client_addr);
		LOG(info) << "a new connection : " << sockaddr.ip << ":" << sockaddr.port;

		const std::string local_ip   = "127.0.0.1";
		TcpConnection*	  connection = new TcpConnection(connfd, sockaddr.ip, sockaddr.port,
								 local_ip, this->port_);
		connection->receive_message_watcher()
			.set< TcpServer, &TcpServer::MessageArrivedCb >(this);
		connection->receive_message_watcher().set(watcher.loop);
		connection->receive_message_watcher().start(connfd, ev::READ);

		connection->send_message_watcher().set< TcpServer, &TcpServer::SendIoWatcherCb >(
			this);
		connection->send_message_watcher().set(watcher.loop);
		if (this->connection_release_cb_)
			connection->SetDisconnectCb(this->connection_release_cb_);
	}
}

void TcpServer::EventLoopThreadFunc(int port) {
	int		listenfd = SocketTool::CreateListenSocket(port, backlog_, false);
	struct ev_loop* evloop	 = ev_loop_new(EVFLAG_AUTO);
	ev::io		listenfd_watch;
	listenfd_watch.set< TcpServer, &TcpServer::ConnectionArrivedCb >(this);
	listenfd_watch.set(evloop);
	listenfd_watch.start(listenfd, ev::READ);
	ev_run(evloop, 0);
}

void TcpServer::DefaultMessageArrivedCb(const TcpConnection& connection, const std::string& msg) {
	LOG(debug) << "new message from : " << connection.remote_ip() << ":"
		   << connection.remote_port() << " : " << msg;

	this->SendMessage(const_cast< TcpConnection* >(&connection), "hi i am a server!", false);
}