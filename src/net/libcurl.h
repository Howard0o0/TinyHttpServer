#ifndef __LIBCURL_H__
#define __LIBCURL_H__
#include <string>

long libcurl_get(const char* url, struct curl_slist* header,
		 std::string& buffer);

long libcurl_post(const char* url, struct curl_slist* header, const char* data,
		  std::string& buffer);

#endif