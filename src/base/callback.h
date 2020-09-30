#ifndef NETHELPER_CALLBACK_H
#define NETHELPER_CALLBACK_H

// #include "tcpconnection.h"
#include <ev.h>
#include <functional>
#include <string>

class TcpConnection;

using Task = std::function< void() >;

using MessageArrivedCallback = std::function< void(TcpConnection&, const std::string&) >;

using TcpConnectionReleaseCallback = std::function< void(TcpConnection&) >;

typedef void (*EviowatchCb)(struct ev_loop* loop, ev_io* watcher, int revents);

#endif