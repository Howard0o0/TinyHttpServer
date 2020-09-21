#ifndef TINYHTTPSERVER_LOG_H
#define TINYHTTPSERVER_LOG_H

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>

#define LOG(level) BOOST_LOG_TRIVIAL(level)
#define LOG_SET_LEVEL(level)                            \
	do {                                            \
		boost::log::core::get()->set_filter(    \
			boost::log::trivial::severity   \
			>= boost::log::trivial::level); \
	} while (0);

void SetLogLevel();

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
