#include "httprequest.h"
#include "curl/curl.h"

long HttpRequest::get(const char* url) {
	return libcurl_get(url, m_header, m_response);
}

long HttpRequest::post(const char* url) {
	return libcurl_post(url, m_header, m_form.c_str(), m_response);
}

std::string HttpRequest::getResponse() {
	return m_response;
}

void HttpRequest::appendHeader(const std::string& data) {
	m_header = curl_slist_append(m_header, data.c_str());
}

void HttpRequest::clearHeader() {
	if (m_header) {
		curl_slist_free_all(m_header);
		m_header = NULL;
	}
}

void HttpRequest::setForm(const std::string& form) {
	m_form = form;
}
