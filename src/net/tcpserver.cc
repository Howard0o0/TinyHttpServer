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
	message_arrived_cb = cb;
}

bool TcpServer::SendMessage(TcpConnection* tcpconnection, const std::string& message,
			    bool close_on_sent) {
	if (message.empty()) {
		if (close_on_sent)
			delete tcpconnection;
		return true;
	}

	int sent_len = send(tcpconnection->connection_fd(), message.c_str(), message.size(), 0);
	if (sent_len == static_cast< int >(message.size())) {
		if (close_on_sent)
			delete tcpconnection;
		return true;
	}
	else if (sent_len == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
		delete tcpconnection;
		return false;
	}
	else {
		std::string rest_of_message = sent_len > 0 ? message.substr(sent_len) : message;
		tcpconnection->PushMessageIntoSendBuffer(rest_of_message, close_on_sent);
		if (!tcpconnection->send_message_watcher().active) {
			LOG(debug) << "start write event watch";
			tcpconnection->send_message_watcher().start(tcpconnection->connection_fd(),
								    ev::WRITE);
		}
		return true;
	}
}

/* private */

void TcpServer::SendIoWatcherCb(ev::io& watcher, int revents) {
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
			delete tcpconnection;
		return;
	}

	TcpConnectionContext* send_context = reinterpret_cast< TcpConnectionContext* >(&watcher);
	send_buffer.sent_offset += sent_len;
	if (send_buffer.IsEmpty()) {
		if (send_context->close_on_sent == true) {
			LOG(debug) << "send buffer empty, disconnect";
			delete tcpconnection;
			return;
		}
		send_buffer.buffer.clear();
		send_buffer.sent_offset = 0;
		watcher.stop();
	}
}

void TcpServer::MessageArrivedCb(ev::io& watcher, int revents) {

	TcpConnection* tcpconnection =
		reinterpret_cast< TcpConnectionContext* >(&watcher)->tcpconnection;
	std::string message = SocketTool::ReadMessage(watcher.fd);
	// if (message.empty()) {
	// 	tcpconnection->Disconnect();
	// 	delete tcpconnection;
	// }
	// else
	this->message_arrived_cb(*tcpconnection, message);
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

		TcpConnection* tcpconnection =
			new TcpConnection(connfd, sockaddr.ip, sockaddr.port);
		tcpconnection->receive_message_watcher()
			.set< TcpServer, &TcpServer::MessageArrivedCb >(this);
		tcpconnection->receive_message_watcher().set(watcher.loop);
		tcpconnection->receive_message_watcher().start(connfd, ev::READ);

		tcpconnection->send_message_watcher().set< TcpServer, &TcpServer::SendIoWatcherCb >(
			this);
		tcpconnection->send_message_watcher().set(
			tcpconnection->receive_message_watcher().loop);
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

void TcpServer::DefaultMessageArrivedCb(const TcpConnection& tcpconnection,
					const std::string&   msg) {
	LOG(debug) << "new message from : " << tcpconnection.remote_ip() << ":"
		   << tcpconnection.remote_port() << " : " << msg;

	this->SendMessage(const_cast< TcpConnection* >(&tcpconnection), "hi i am a server!", false);
}