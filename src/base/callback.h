#ifndef NETHELPER_CALLBACK_H
#define NETHELPER_CALLBACK_H

#include <functional>

typedef std::function< void() >					  Task;
typedef std::function< void(int sockfd, const std::string& msg) > OnMsgCallback;

#endif