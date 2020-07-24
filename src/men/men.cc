#include "men.h"
#include "httprequest.h"
#include "log.h"
#include <re2/re2.h>

void Men::OnMsgArrived(int client_fd, const std::string& message) {

	LOG_DEBUG("received buf: \n %s \n", message.c_str());

	std::string params_str = FetchParamsStr(message);
	if (params_str.empty()) {
		ResponseClient(client_fd, 404);
		return;
	}

	std::string result = ScrawResult(params_str);
	if (result.empty()) {
		ResponseClient(client_fd, 404);
		return;
	}

	ResponseClient(client_fd, 200, result);
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

	std::string url		= "http://man.he.net/" + params;
	long	    status_code = request.get(url.c_str());
	if (status_code == 200) {
		std::string content = request.getResponse();
		RE2::GlobalReplace(&content, "<.*?>", "");
		// std::ofstream ofs("ab.md", std::ios::out);
		// ofs << content;
		std::cout << content << std::endl;
		return content;
		// ofs.close();
	}
}