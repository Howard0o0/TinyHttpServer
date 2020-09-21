#include "log.h"


void SetLogLevel() {
	boost::log::core::get()->set_filter(boost::log::trivial::severity
					    >= boost::log::trivial::info);
}