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

/* private */

void TcpServer::MessageArrivedCb(ev::io& watcher, int revents) {
	LOG(trace) << "new message from fd:" << watcher.fd;

	TcpConnection* tcpconnection = reinterpret_cast< TcpConnection* >(&watcher);
	std::string    message	     = SocketTool::ReadMessage(watcher.fd);
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
