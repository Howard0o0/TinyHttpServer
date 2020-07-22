#include "libcurl.h"
#include "curl/curl.h"
#include "muduo/base/Logging.h"
#include <iostream>
#include <string>

static char   error_buffer[ CURL_ERROR_SIZE ];
static size_t writer(char*, size_t, size_t, std::string*);
static bool   init(CURL*&, const char*, std::string*);
const long    g_timeout = 5;

static bool init(CURL*& conn, const char* url, std::string* p_buffer) {
	CURLcode code;

	// conn = curl_easy_init();
	if (NULL == conn) {
		LOG_ERROR << " Failed to create CURL connection";
		exit(EXIT_FAILURE);
	}

	code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, error_buffer);
	if (code != CURLE_OK) {
		LOG_ERROR << " Failed to set error buffer " << code;
		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_URL, url);
	if (code != CURLE_OK) {
		LOG_ERROR << " Failed to set URL "
			  << "-->" << error_buffer;
		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1);
	if (code != CURLE_OK) {
		LOG_ERROR << " Failed to set redirect option "
			  << "-->" << error_buffer;
		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
	if (code != CURLE_OK) {
		LOG_ERROR << " Failed to set writer "
			  << "-->" << error_buffer;
		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, p_buffer);
	if (code != CURLE_OK) {
		LOG_ERROR << " Failed to set write data "
			  << "-->" << error_buffer;
		return false;
	}

	curl_easy_setopt(conn, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(conn, CURLOPT_SSL_VERIFYHOST, 0L);

	curl_easy_setopt(conn, CURLOPT_TIMEOUT, g_timeout);
	curl_easy_setopt(conn, CURLOPT_NOSIGNAL, 1L);

	return true;
}

static size_t writer(char* data, size_t size, size_t nmemb,
		     std::string* writer_data) {
	size_t sizes = size * nmemb;

	if (NULL == writer_data) {
		return 0;
	}

	writer_data->append(data, sizes);

	return sizes;
}

static std::string GetRedirectUrl(const char* url) {
	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL* curl = curl_easy_init();
	if (curl) {
		char* res = NULL;
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_perform(curl);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		// 1表示重定向次数，最多允许一次重定向
		std::cout << curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, res)
			  << std::endl;
		//该函数若返回301，说明是永久重定向；若返回302，说明临时重定向
		printf("hah:%s\n", res);
		std::string str(res);
		curl_easy_cleanup(curl);  //防止内存泄漏 切记
		return str;
	}
	else {
		return "";
	}
}

long libcurl_get(const char* url, struct curl_slist* header,
		 std::string& buffer) {

	CURL*    conn = NULL;
	CURLcode code;
	long     status_code = -1;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	conn = curl_easy_init();
	buffer.clear();
	if (!init(conn, url, &buffer)) {
		LOG_ERROR << " Connection initializion failed";

		return status_code;
	}

	curl_easy_setopt(conn, CURLOPT_HTTPHEADER, header);
	curl_easy_setopt(conn, CURLOPT_HTTPGET, true);
	curl_easy_setopt(
		conn, CURLOPT_USERAGENT,
		"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, "
		"like Gecko) Chrome/79.0.3945.88 Safari/537.36");

	code = curl_easy_perform(conn);

	if (code != CURLE_OK) {
		LOG_ERROR << " Failed to get " << url << "-->" << error_buffer;

		return status_code;
	}

	curl_easy_getinfo(conn, CURLINFO_RESPONSE_CODE, &status_code);
	curl_easy_cleanup(conn);

	return status_code;
}

long libcurl_post(const char* url, struct curl_slist* header, const char* data,
		  std::string& buffer) {
	CURL*    conn = NULL;
	CURLcode code;
	long     status_code = -1;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	conn = curl_easy_init();

	buffer.clear();
	if (!init(conn, url, &buffer)) {
		LOG_ERROR << " Connection initializion failed";

		return status_code;
	}

	code = curl_easy_setopt(conn, CURLOPT_POST, true);

	if (code != CURLE_OK) {
		LOG_ERROR << " Failed to set CURLOPT_POST ";
		return status_code;
	}

	code = curl_easy_setopt(conn, CURLOPT_POSTFIELDS, data);
	if (code != CURLE_OK) {
		LOG_ERROR << " Failed to set CURLOPT_POSTFIELDS ";
		return status_code;
	}

	curl_easy_setopt(conn, CURLOPT_HTTPHEADER, header);
	code = curl_easy_perform(conn);

	if (code != CURLE_OK) {
		LOG_ERROR << " Failed to post " << url << "-->" << error_buffer;

		return status_code;
	}

	curl_easy_getinfo(conn, CURLINFO_RESPONSE_CODE, &status_code);
	curl_easy_cleanup(conn);

	return status_code;
}
