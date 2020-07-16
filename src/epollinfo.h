#ifndef TINYHTTPSERVER_EPOLLINFO_H
#define TINYHTTPSERVER_EPOLLINFO_H

#include <sys/epoll.h>

struct EpollInfo {
	int		    epollfd;
	struct epoll_event  event;
	struct epoll_event* active_events;
	static const int    MAX_EVENTS_CNT = 50;
};

#endif