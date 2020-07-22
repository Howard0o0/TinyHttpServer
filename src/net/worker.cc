#include "worker.h"
#include "epolltool.h"
#include "log.h"
#include "muduo/base/Thread.h"
#include "thread.h"
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

using namespace nethelper;

/* public */
Worker::Worker(const OnMsgCallback& cb, int thread_cnt)
	: thread_cnt_(thread_cnt), epollfds_(thread_cnt, 0),
	  expired_fds(10000, 0) {
	on_msg_cb_ = cb;
	threadpool_.Start(thread_cnt_);
	for (int weak_threadid = 0; weak_threadid < thread_cnt;
	     ++weak_threadid) {
		EpollInfo epollinfo;
		EpollTool::InitialEpollinfo(epollinfo);
		epollfds_[ weak_threadid ] = epollinfo.epollfd;
		LOG_DEBUG("created epollfd : %d \n",
			  epollfds_[ weak_threadid ]);

		threadpool_.RunTask(std::bind(&Worker::HandleClientFd, this, 0,
					      weak_threadid));

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
			if (message.size() == 0) {
				EpollTool::DelEpoll(active_fd, epollinfo);
				close(active_fd);
				continue;
			}

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
