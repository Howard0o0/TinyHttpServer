#include "epolltool.h"
#include <vector>
#include <stdlib.h>



	void EpollTool::InitialEpollinfo(EpollInfo& epoll_info){
			epoll_info.epollfd       = epoll_create(100);
	epoll_info.event.events  = EPOLLIN | EPOLLET;
	epoll_info.active_events = ( struct epoll_event* )calloc(
		EpollInfo::MAX_EVENTS_CNT, sizeof(epoll_event));
	}
	void		   EpollTool::RegisterEpoll(int fd, EpollInfo& epoll_info){

	}
	void EpollTool::DelEpoll(int fd,EpollInfo& epoll_info){

	}
	std::vector< int > EpollTool::GotEpollActiveFd(EpollInfo& epoll_info){

	}