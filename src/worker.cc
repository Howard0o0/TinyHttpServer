#include "worker.h"
#include "epolltool.h"
#include "log.h"
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

using namespace ths;

/* public */
Worker::Worker(const OnMsgCallback& cb, int thread_cnt)
	: thread_cnt_(thread_cnt), epollfds_(thread_cnt, 0),
	  expired_fds(10000, 0) {
	on_msg_cb_ = cb;
	// threadpool_.Start(thread_cnt_);
	for (int weak_threadid = 0; weak_threadid < thread_cnt;
	     ++weak_threadid) {
		EpollInfo epollinfo;
		EpollTool::InitialEpollinfo(epollinfo);
		epollfds_[ weak_threadid ] = epollinfo.epollfd;
		LOG_DEBUG("created epollfd : %d \n",
			  epollfds_[ weak_threadid ]);
		// threadpool_.RunTask(std::bind(&Worker::HandleClientFd, this,
		// 0, 			      weak_threadid));
		std::thread t1 = std::thread(std::bind(&Worker::HandleClientFd,
						       this, 0, weak_threadid));
		t1.detach();
		LOG_DEBUG("created thread\n");
	}
}

void Worker::SetOnMessageCallback(const OnMsgCallback& cb) {
	on_msg_cb_ = cb;
}
void Worker::HandleResponse(int client_fd) {
	/* throw into threadpool to handle */
	static int weak_thread_id = 0;
	weak_thread_id		  = (weak_thread_id + 1) % thread_cnt_;

	static EpollInfo epollinfo;
	epollinfo.epollfd = epollfds_[ weak_thread_id ];
	EpollTool::RegisterEpoll(client_fd, epollinfo);
	LOG_DEBUG("register clientfd(%d) into epollfd(%d) \n", client_fd,
		  epollinfo.epollfd);
}

/* private  */

void Worker::HandleClientFd(int client_fd, int weak_thread_id) {

	EpollInfo epollinfo;
	epollinfo.epollfd = epollfds_[ weak_thread_id ];
	while (1) {
		LOG_DEBUG("epoll wait : %d \n", epollinfo.epollfd);
		std::vector< int > active_fds =
			EpollTool::GotEpollActiveFd(epollinfo, 2000);
		for (auto active_fd : active_fds) {
			/* read message */
			std::string message = ReadMsg(active_fd);

			EpollTool::DelEpoll(active_fd, epollinfo);
			OnMsgArrived(active_fd, message);
		}
	}
}
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
		// printf("readbuf:%s\n", readbuf);
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
		LOG_DEBUG("epoll wait return \n");
		for (int i = 0; i < active_events_cnt; ++i) {
			LOG_DEBUG("[%d] active cnt:%d,  active event fd: %d\n",
				  i, active_events_cnt,
				  active_events[ i ].data.fd);
			if (wakeup_fd_ == active_events[ i ].data.fd) {
				char temp[ 10 ];
				read(wakeup_fd_, temp, 10);

				LOG_DEBUG("new connection arrived\n");
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
				continue;
			}
			LOG_DEBUG("new msg:\n");
			LOG_DEBUG("%s\n", msg.data());
			on_msg_cb_(active_events[ i ].data.fd, msg);
		}
	}
}