#ifndef TINYHTTPSERVER_LOG_H
#define TINYHTTPSERVER_LOG_H

#include <iostream>
#include <boost/log/trivial.hpp>

#define LOG(level) BOOST_LOG_TRIVIAL(level)

// #define __LOG_LEVEL_DEBUG__
#define __LOG_LEVEL_INFO__
#define __LOG_LEVEL_ERR__

#ifdef __LOG_LEVEL_DEBUG__
#define LOG_DEBUG(...) printf(__VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif

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
