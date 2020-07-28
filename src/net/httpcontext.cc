#include "httpcontext.h"

void HttpContext::SetHeader(const std::string& key, const std::string& value) {
	headers_[ key ] = value;
}
void HttpContext::SetBody(const std::string& body) {
	body_ = body;
}
void HttpContext::SetStatuscode(int statuscode) {
	std::string status_msg = GetStatusmessage(statuscode);
	header_ = "HTTP/1.1 " + std::to_string(statuscode) + " " + status_msg
		  + "\r\n";
}
std::string HttpContext::GetHttpContextStr() {
	std::string context = header_;
	for (auto block : headers_) {
		std::string header_line =
			block.first + ":" + block.second + "\r\n";
		context += header_line;
	}
	if (body_.empty())
		body_ = "not found";
	body_ += "\r\n";
	context += "content-length:" + std::to_string(body_.size()) + "\r\n";
	context += "\r\n";
	return context + body_;
}
void HttpContext::Parse(const std::string& message) {
}

/* private */

std::string HttpContext::GetStatusmessage(int statuscode) {
	switch (statuscode) {
	case 200:
		return "OK";
		break;
	case 404:
		return "Not Found";
		break;

	default:
		return "";
		break;
	}
}