#include "epolltool.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>

void EpollTool::InitialEpollinfo(EpollInfo& epoll_info) {
	epoll_info.epollfd = epoll_create1(0);
	if (epoll_info.epollfd <= 0) {
		LOG_ERR("create epollfd error : %d , %s \n", epoll_info.epollfd,
			strerror(errno));
		exit(-1);
	}
}
void EpollTool::RegisterEpoll(int fd, EpollInfo& epoll_info, uint32_t events) {
	epoll_info.event.data.fd = fd;
	epoll_info.event.events	 = events;
	// epoll_info.event.events = EPOLLIN;

	if (epoll_ctl(epoll_info.epollfd, EPOLL_CTL_ADD, fd, &epoll_info.event)
	    != 0) {
		LOG_ERR("epoll add fd error: %s \n", strerror(errno));
		LOG_ERR("epollfd: %d, add fd:%d\n", epoll_info.epollfd, fd);
		// exit(-1);
	}
}
void EpollTool::DelEpoll(int fd, EpollInfo& epoll_info) {
	epoll_info.event.data.fd = fd;

	if (epoll_ctl(epoll_info.epollfd, EPOLL_CTL_DEL, fd, NULL) != 0) {
		LOG_ERR("epoll delete fd error: %s \n", strerror(errno));
		// exit(-1);
	}
}
std::vector< int > EpollTool::GotEpollActiveFd(EpollInfo& epoll_info,
					       int	  timeout) {
	int active_events_cnt =
		epoll_wait(epoll_info.epollfd, epoll_info.active_events,
			   EpollInfo::MAX_EVENTS_CNT, timeout);
	std::vector< int > active_fds;
	for (int i = 0; i < active_events_cnt; ++i) {
		if (epoll_info.active_events[ i ].events & EPOLLIN)
			active_fds.push_back(
				epoll_info.active_events[ i ].data.fd);
		else {
			DelEpoll(epoll_info.active_events[ i ].data.fd,
				 epoll_info);
			close(epoll_info.active_events[ i ].data.fd);
		}
	}
	return active_fds;
}