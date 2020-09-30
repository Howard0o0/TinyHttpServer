#include "httpmessagecodec.h"
#include "regextool.h"

/* public methods */
enum HttpProxyMessageType HttpMessageCodec::GetHttpProxyMessageType(const std::string& message) {

	std::regex pattern_connect("^CONNECT", std::regex_constants::icase);
	std::regex pattern_forward("Host", std::regex_constants::icase);
	if (std::regex_search(message, pattern_connect))
		return CONNECT;
	else if (std::regex_search(message, pattern_forward))
		return HTTPFORWARD;
	else
		return FORWARD;
}

nethelper::SockAddress HttpMessageCodec::ScratchRemoteAddress(const std::string& message) {

	std::regex		    pattern("Host\\s*:\\s*([\\w|\\.]*)\\s*:?\\s*(\\d*)\\b",
			    std::regex_constants::icase);
	std::smatch		    results;
	std::string::const_iterator iter_begin = message.begin();
	std::string::const_iterator iter_end   = message.end();

	if (!std::regex_search(iter_begin, iter_end, results, pattern))
		return nethelper::SockAddress();

	std::string ip_address = results[ 1 ].str();
	uint16_t    port       = results[ 2 ].str().empty() ? 80 : atoi(results[ 2 ].str().c_str());
	return nethelper::SockAddress(ip_address, port);
}

bool HttpMessageCodec::RefactorUrlpath(std::string& message) {
	std::string		   url_pattern("((https?|ftp|file)://[-A-Za-z0-9+&@#/"
				       "%?=~_|!:,.;]+[-A-Za-z0-9+&@#/%=~_|])");
	std::vector< std::string > match_results;
	if ((match_results = RegexTool::RegexMatch(message, url_pattern, true, 2)).empty())
		return false;
	std::string url = match_results[ 0 ];

	std::string url_file_path_pattern = ".*//[\\w|\\.]*(/.*)";
	std::string url_file_path =
		RegexTool::RegexMatch(url, url_file_path_pattern, true, 1).front();
	// std::cout << url_file_path << std::endl;

	auto start_pos = message.find(url);
	message.replace(start_pos, url.length(), url_file_path);

	// std::cout << "url:" << url << ",file_path:" << url_file_path << std::endl;

	return true;
}

/* end of public methods */