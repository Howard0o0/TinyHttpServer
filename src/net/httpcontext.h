#ifndef NETHELPER_HTTPCONTEXT_H
#define NETHELPER_HTTPCONTEXT_H

#include <string>
#include <unordered_map>
#include <utility>

class HttpContext {
    public:
	void	    SetHeader(const std::string& key, const std::string& value);
	void	    SetBody(const std::string& body);
	void	    SetStatuscode(int statuscode);
	std::string GetHttpContextStr();
	void	    Parse(const std::string& message);

    private:
	std::string				       header_;
	std::unordered_map< std::string, std::string > headers_;
	std::string				       body_;

	std::string GetStatusmessage(int statuscode);
};

#endif