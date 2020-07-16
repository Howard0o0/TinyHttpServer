#include "worker.h"
#include "log.h"
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace ths;

/* public */
Worker::Worker(const OnMsgCallback& cb) {
	on_msg_cb_ = cb;
	wakeup_fd_ = socket(AF_UNIX, SOCK_STREAM, 0);
	LOG_DEBUG("wakeup_fd: %d\n", wakeup_fd_);
	threadpool_.Start(4);
}

void Worker::SetOnMessageCallback(const OnMsgCallback& cb) {
	on_msg_cb_ = cb;
}
void Worker::HandleResponse(int client_fd) {

	/* read message */
	std::string message = ReadMsg(client_fd);

	/* if socket was close by client, server close too */
	if (message.empty()) {
		LOG_INFO("close fd[%d]\n", client_fd);
		close(client_fd);
		return;
	}

	/* throw into threadpool to handle */
	threadpool_.RunTask(
		std::bind(&Worker::OnMsgArrived, this, client_fd, message));
}

/* private  */

void Worker::OnMsgArrived(int sockfd, std::string msg) {
	on_msg_cb_(sockfd, msg);
}

std::string Worker::ReadMsg(int client_fd) {
	const int   READBUF_LEN = 1024;
	char	readbuf[ READBUF_LEN ];
	int	 read_len;
	std::string msg = "";
	while ((read_len = recv(client_fd, readbuf, READBUF_LEN, MSG_DONTWAIT))
	       > 0) {
		readbuf[ read_len ] = '\0';
		msg += readbuf;
		printf("readbuf:%s\n", readbuf);
	}
	return msg;
}

void Worker::WorkFunc() {
	int		    epollfd = epoll_create(100);
	struct epoll_event  event;
	struct epoll_event* active_events;
	event.data.fd = wakeup_fd_;
	event.events  = EPOLLIN | EPOLLET;

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, wakeup_fd_, &event) != 0) {
		LOG_ERR("epoll add wakeup_fd error: %s", strerror(errno));
		exit(-1);
	}
	const int MAX_EVENTS_CNT = 50;
	active_events		 = ( struct epoll_event* )calloc(MAX_EVENTS_CNT,
							 sizeof(epoll_event));
	while (1) {
		std::cout << std::this_thread::get_id() << " is waiting epoll"
			  << std::endl;
		int active_events_cnt =
			epoll_wait(epollfd, active_events, MAX_EVENTS_CNT, -1);
		LOG_INFO("epoll wait return \n");
		for (int i = 0; i < active_events_cnt; ++i) {
			LOG_INFO("[%d] active cnt:%d,  active event fd: %d\n",
				 i, active_events_cnt,
				 active_events[ i ].data.fd);
			if (wakeup_fd_ == active_events[ i ].data.fd) {
				char temp[ 10 ];
				read(wakeup_fd_, temp, 10);

				LOG_INFO("new connection arrived\n");
				/* new connection arrived */
				int client_fd = fd_translator_.PopInThread();

				printf("got new client_fd : %d\n", client_fd);
				if (client_fd > 0) {
					event.data.fd = client_fd;
					event.events  = EPOLLIN | EPOLLET;
					epoll_ctl(epollfd, EPOLL_CTL_ADD,
						  client_fd, &event);
				}

				continue;
			}

			std::string msg = ReadMsg(active_events[ i ].data.fd);
			if (msg.empty()) {
				/* client_fd was closed by client */
				int expired_fd = active_events[ i ].data.fd;
				epoll_ctl(epollfd, EPOLL_CTL_DEL, expired_fd,
					  NULL);
				// close(expired_fd);
				LOG_INFO("close expired fd:%d\n", expired_fd);
				continue;
			}
			LOG_INFO("new msg:\n");
			LOG_INFO("%s\n", msg.data());
			on_msg_cb_(active_events[ i ].data.fd, msg);
		}
	}
}