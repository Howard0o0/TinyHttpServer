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
		if (tcpconnection->send_message_watcher().active)
			tcpconnection->send_message_watcher().set(
				tcpconnection->receive_message_watcher().loop);
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
			delete tcpconnection;
			return;
		}
		send_buffer.buffer.clear();
		send_buffer.sent_offset = 0;
		watcher.stop();
	}
}

void TcpServer::MessageArrivedCb(ev::io& watcher, int revents) {
	LOG(trace) << "new message from fd:" << watcher.fd;

	TcpConnection* tcpconnection =
		reinterpret_cast< TcpConnectionContext* >(&watcher)->tcpconnection;
	std::string message = SocketTool::ReadMessage(watcher.fd);
	LOG(debug) << message;
	if (message.empty()) {
		tcpconnection->Disconnect();
		delete tcpconnection;
	}
	else
		this->message_arrived_cb(*tcpconnection, message);
}

void TcpServer::ConnectionArrivedCb(ev::io& watcher, int revents) {
	LOG(trace) << "create a new connection";
	int connfd = -1;
	while ((connfd = accept4(watcher.fd, NULL, NULL, SOCK_NONBLOCK)) > 0) {
		TcpConnection* tcpconnection = new TcpConnection(connfd);
		tcpconnection->receive_message_watcher()
			.set< TcpServer, &TcpServer::MessageArrivedCb >(this);
		tcpconnection->receive_message_watcher().set(watcher.loop);
		tcpconnection->receive_message_watcher().start(connfd, ev::READ);
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
