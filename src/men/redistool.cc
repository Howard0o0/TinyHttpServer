#include "redistool.h"
#include "log.h"

/* publci */

bool RedisTool::ConnectDatabase(const std::string& ip, int port) {
	redisContext* redis_context = redisConnect(ip.c_str(), port);

	if (!redis_context) {
		LOG_ERR("can't allocate redis context!\r\n");
		return false;
	}

	if (redis_context->err) {
		LOG_ERR("redis connection error:%s\n", redis_context->errstr);
		redisFree(redis_context);
		return false;
	}

	redis_context_.reset(redis_context, RedisTool::RediscontextDeleter);

	return true;
}

bool RedisTool::IsConnected() {
	if (redis_context_)
		return true;
	else
		return false;
}
bool RedisTool::SetString(const std::string& key, const std::string& value) {
	if (!redis_context_) {
		LOG_ERR("redis_context_ is null\n");
		return false;
	}

	redisReply* reply = ( redisReply* )redisCommand(
		redis_context_.get(), "SET %s %s", key.c_str(), value.c_str());
	std::shared_ptr< redisReply > redis_reply(reply,
						  RedisTool::RedisReplyDeleter);

	if (!redis_reply || redis_reply->type == REDIS_REPLY_ERROR) {
		LOG_ERR("get redis reply error\n");
		return false;
	}

	return true;
}

std::string RedisTool::GetString(const std::string& key) {
	redisReply* reply = ( redisReply* )redisCommand(redis_context_.get(),
							"GET %s", key.c_str());
	std::shared_ptr< redisReply > redis_reply(reply,
						  RedisTool::RedisReplyDeleter);

	if (!redis_reply || redis_reply->type == REDIS_REPLY_ERROR) {
		LOG_ERR("get redis reply error\n");
		return "";
	}
	if (redis_reply->len == 0)
		return "";

	// LOG_ERR("redis get key:%s (%s)\n", key.c_str(), redis_reply->str);
	return std::string(redis_reply->str);
}

/* private */

void RedisTool::RediscontextDeleter(redisContext* redis_context) {
	if (redis_context)
		redisFree(redis_context);
}
void RedisTool::RedisReplyDeleter(redisReply* redis_reply) {
	if (redis_reply)
		freeReplyObject(redis_reply);
}