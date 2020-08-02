#include "men.h"
#include "httpcontext.h"
#include "httprequest.h"
#include "log.h"
#include <fcntl.h>
#include <re2/re2.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>

/* private */

void Men::OnMsgArrived(int client_fd, const std::string& message) {

	LOG_DEBUG("received buf: \n %s \n", message.c_str());

	std::string params_str = FetchParamsStr(message);
	LOG_DEBUG("params_str : %s\n", params_str.c_str());

	if (params_str.empty()) {
		ResponseClient(client_fd, 200, "params error\r\n");
		return;
	}
	if (params_str == "/") {
		ThreadPool::RunTaskInGlobalThreadPool(std::bind(
			&Men::SendFile, this, client_fd, "index.html"));
		// SendFile(client_fd, "index.html");
		return;
	}

	std::string topic = FetchTopic(params_str);
	if (topic.empty()) {
		ThreadPool::RunTaskInGlobalThreadPool(
			std::bind(&Men::ResponseClient, this, client_fd, 200,
				  "params error\r\n"));
		// ResponseClient(client_fd, 200, "params error\r\n");
		return;
	}
	// LOG_DEBUG("topic: %s\n", topic.c_str());
	std::string result;

	/* if result existed in cache */
	result = TryGetResultFromCache(topic);
	if (!result.empty()) {
		ThreadPool::RunTaskInGlobalThreadPool(
			std::bind(&Men::ResponseClient, this, client_fd, 200,
				  result + "\r\n"));
		// ResponseClient(client_fd, 200, result);
		return;
	}

	LOG_DEBUG("cache miss\n");
	/* result not existed in cache,scraw result from web */
	result = ScrawResult(topic);
	if (result.empty()) {
		LOG_DEBUG("scraw failed\n");
		ThreadPool::RunTaskInGlobalThreadPool(
			std::bind(&Men::ResponseClient, this, client_fd, 200,
				  "not found\r\n"));
		// ResponseClient(client_fd, 404, "not found\r\n");
		return;
	}
	ThreadPool::RunTaskInGlobalThreadPool(std::bind(
		&Men::ResponseClient, this, client_fd, 200, result + "\r\n"));
	// ResponseClient(client_fd, 200, result);
	SaveIntoCache(topic, result);
}

RedisTool* Men::GetCurrRedistool() {

	if (tid_to_redistool_.count(nethelper::Thread::GetThreadid()) == 0)
		tid_to_redistool_[ nethelper::Thread::GetThreadid() ] =
			RedisTool();
	RedisTool& redistool =
		tid_to_redistool_[ nethelper::Thread::GetThreadid() ];

	if (!redistool.IsConnected()
	    && redistool.ConnectDatabase(redis_ip_, redis_port_) == false)
		return nullptr;

	return &redistool;
}

void Men::SaveIntoCache(const std::string& key, const std::string& value) {

	RedisTool* redistool = GetCurrRedistool();
	if (!redistool)
		return;

	redistool->SetString(key, value);
}

std::string Men::TryGetResultFromCache(const std::string& key) {
	RedisTool* redistool = GetCurrRedistool();
	if (!redistool)
		return "";

	return redistool->GetString(key);
}

std::string Men::ScrawResult(const std::string& params) {

	HttpRequest request;
	request.clearHeader();
	request.appendHeader("Host:man.he.net");
	request.appendHeader("Cache-Control: no-cache");
	request.appendHeader("Accept: "
			     "text/html,application/xhtml+xml,application/"
			     "xml;q=0.9,image/webp,image/apng,*/"
			     "*;q=0.8,application/signed-exchange;v=b3;q=0.9");
	request.appendHeader("Accept-Encoding: gzip, deflate, br");

	std::string url = "http://man.he.net/?topic=" + params + "&section=all";
	long	    status_code = request.get(url.c_str());
	if (status_code == 200) {
		std::string content = request.getResponse();
		if (content.size() < 600)
			return "";

		RE2::GlobalReplace(&content, "<.*>", "");
		RE2::GlobalReplace(&content, "^\\s+|\\s+$", "");
		// std::ofstream ofs("ab.md", std::ios::out);
		// ofs << content;
		LOG_DEBUG("scraw : \n%s\n", content.c_str());
		return content;
		// ofs.close();
	}
	return "";
}

std::string Men::FetchTopic(const std::string& params) {

	re2::StringPiece group[ 2 ];
	RE2		 re("topic=(.*)");
	std::string	 content = "/?topic=ab&section=all";
	std::string	 result;
	re.Match(params, 0, params.size(), RE2::UNANCHORED, group, 2);

	return group[ 1 ].as_string();
}

/*  */