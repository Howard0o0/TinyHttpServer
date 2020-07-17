#ifndef TINYHTTPSERVER_EPOLLTOOL_H
#define TINYHTTPSERVER_EPOLLTOOL_H

#include <sys/epoll.h>

struct EpollInfo {
	int		    epollfd;
	struct epoll_event  event;
	struct epoll_event* active_events;
	static const int    MAX_EVENTS_CNT = 50;
};


class EpollTool{
public:
	static void		   InitialEpollinfo(EpollInfo& epoll_info);
	static void		   RegisterEpoll(int fd,EpollInfo& epoll_info);
	static void DelEpoll(int fd,EpollInfo& epoll_info);
	static std::vector< int > GotEpollActiveFd(EpollInfo& epoll_info);
};


#endif