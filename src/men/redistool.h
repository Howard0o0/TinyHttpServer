#ifndef MEN_REDISTOOL_H
#define MEN_REDISTOOL_H

#include "hiredis/hiredis.h"
#include <memory>
#include <string>

class RedisTool {

    public:
	bool	    ConnectDatabase(const std::string& ip, int port);
	bool	    IsConnected();
	bool	    SetString(const std::string& key, const std::string& value);
	std::string GetString(const std::string& key);

    private:
	std::shared_ptr< redisContext > redis_context_;

	static void RediscontextDeleter(redisContext* redis_context);
	static void RedisReplyDeleter(redisReply* redis_reply);
};

#endif