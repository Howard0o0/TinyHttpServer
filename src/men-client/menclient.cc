#include "menclient.h"

std::string MenClient::Query(const std::string& topic) {
	if (topic.empty())
		return "";

	HttpRequest request;
	request.clearHeader();
	std::string host = "Host:" + server_ip_;
	request.appendHeader(host);
	request.appendHeader("Accept: "
			     "text/plain,application/xhtml+xml,application/"
			     "xml;q=0.9,image/webp,image/apng,*/"
			     "*;q=0.8,application/signed-exchange;v=b3;q=0.9");
	request.appendHeader("Accept-Encoding: gzip, deflate, br");

	std::string url = "http://" + server_ip_ + ":"
			  + std::to_string(server_port_) + "/?topic=" + topic
			  + "&section=all";
	long status_code = request.get(url.c_str());
	if (status_code != 200) {

		return "";
	}

	return request.getResponse();
	std::string content = request.getResponse();
}
