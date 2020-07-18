#ifndef TINYHTTPSERVER_EPOLLTOOL_H
#define TINYHTTPSERVER_EPOLLTOOL_H

#include <stdlib.h>
#include <sys/epoll.h>
#include <vector>

struct EpollInfo {
	int		    epollfd;
	struct epoll_event  event;
	struct epoll_event* active_events;
	static const int    MAX_EVENTS_CNT = 1024;
	EpollInfo() {
		active_events = ( struct epoll_event* )calloc(
			EpollInfo::MAX_EVENTS_CNT, sizeof(epoll_event));
	}
	~EpollInfo() {
		free(active_events);
	}
};

class EpollTool {
    public:
	static void		  InitialEpollinfo(EpollInfo& epoll_info);
	static void		  RegisterEpoll(int fd, EpollInfo& epoll_info);
	static void		  DelEpoll(int fd, EpollInfo& epoll_info);
	static std::vector< int > GotEpollActiveFd(EpollInfo& epoll_info,
						   int	timeout = 0);
};

#endif