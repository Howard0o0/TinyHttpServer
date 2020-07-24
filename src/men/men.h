#ifndef MEN_MEN_H
#define MEN_MEN_H

#include "httpserver.h"
#include "redistool.h"
#include <string>
#include <unordered_map>

using namespace nethelper;

class Men : public HttpServer {
    public:
	Men(int port, int threadnum = 1,
	    const std::string& redis_ip	  = "127.0.0.1",
	    const int	       redis_port = 6379)
		: HttpServer(port, threadnum), redis_ip_(redis_ip),
		  redis_port_(redis_port) {
	}

    private:
	std::unordered_map< int, RedisTool > tid_to_redistool_;
	const std::string		     redis_ip_;
	const int			     redis_port_;

    private:
	void OnMsgArrived(int client_fd, const std::string& message) override;
	std::string ScrawResult(const std::string& params);
	std::string FetchTopic(const std::string& params);
	std::string TryGetResultFromCache(const std::string& key);
	void SaveIntoCache(const std::string& key, const std::string& value);
	RedisTool* GetCurrRedistool();
};

#endif