#ifndef NETHELPER_CALLBACK_H
#define NETHELPER_CALLBACK_H

#include "tcpconnection.h"
#include <ev.h>
#include <functional>
#include <string>

typedef std::function< void() > Task;
// typedef std::function< void(int sockfd, const std::string& msg) >
// MessageArrivedCallback;
// typedef std::function< void(const TcpConnection& tcp_connection,
// 			    const std::string&	 msg) >
// 		    MessageArrivedCallback;
using MessageArrivedCallback =
	std::function< void(TcpConnection&, std::string) >;
// typedef std::function< void(TcpConnection, std::string) >
// MessageArrivedCallback;

// using EviowatchCb = std::function< void(struct ev_loop*, ev_io*, int) >;
typedef void (*EviowatchCb)(struct ev_loop* loop, ev_io* watcher, int revents);

#endif