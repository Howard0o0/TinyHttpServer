#ifndef TINYHTTPSERVER_LOG_H
#define TINYHTTPSERVER_LOG_H

#include <iostream>

#define __LOG_LEVEL_INFO__
#define __LOG_LEVEL_ERR__

#ifdef __LOG_LEVEL_INFO__
#define LOG_INFO(...) printf(__VA_ARGS__)
#else
#define LOG_INFO(...)
#endif

#ifdef __LOG_LEVEL_ERR__
#define LOG_ERR(...) printf(__VA_ARGS__)
#else
#define LOG_ERR(...)
#endif

#endif
