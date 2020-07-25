#ifndef __HTTPREQUEST_H
#define __HTTPREQUEST_H

#include "libcurl.h"

class HttpRequest {
    public:
	typedef struct curl_slist* header_t;

	HttpRequest() : m_header(NULL) {
	}
	~HttpRequest() {
		clearHeader();
		m_header = NULL;
	}
	HttpRequest(const HttpRequest& req) : m_header(NULL) {
	}
	long	get(const char* url);
	long	post(const char* url);
	std::string getResponse();
	void	appendHeader(const std::string& data);
	void	clearHeader();
	void	setForm(const std::string& form);

    private:
	header_t    m_header;
	std::string m_response;
	std::string m_form;
};

#endif