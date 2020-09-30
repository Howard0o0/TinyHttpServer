#ifndef SHADOWHTTP_HTTPMESSAGECODEC_H
#define SHADOWHTTP_HTTPMESSAGECODEC_H

#include "sockettool.h"
#include <string>

enum HttpProxyMessageType { CONNECT, FORWARD, HTTPFORWARD };

class HttpMessageCodec {
    public:
	enum HttpProxyMessageType GetHttpProxyMessageType(const std::string& message);
	nethelper::SockAddress	  ScratchRemoteAddress(const std::string& message);
	bool			  RefactorUrlpath(std::string& message);

    private:
};

#endif

// CONNECT www.google.com:443 HTTP/1.1
// Host: www.google.com:443
// Proxy-Connection: keep-alive
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_6) AppleWebKit/537.36 (KHTML, like
// Gecko) Chrome/85.0.4183.121 Safari/537.36

//  *GET http://google.com/ HTTP/1.1
// Host: google.com
// Proxy-Connection: keep-alive
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_6) AppleWebKit/537.36 (KHTML, like
// Gecko) Chrome/85.0.4183.121 Safari/537.36 Accept:
// text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,
// *;
// q = 0.8, application / signed - exchange;
// v = b3;
// q = 0.9 Accept - Encoding : gzip, deflate Accept - Language : zh - CN, zh;
// q = 0.9, en;
// q = 0.8 Cookie : SID = 1Ae9A2mysa8GPVyo4K8u6BzKUX358QfnSxGkvXLSh8E9s4Zfe_5KevXVtZ6VVLek - p2nCA.;
// HSID		     = AG2VxxBTN0jVw3ikl;
// APISID		     = kupt5StlAhsVe7DO / AE0BAT1TkH3FEUvGP;
// SEARCH_SAMESITE	     = CgQIzZAB;
// SIDCC = AJi4QfEzAvzxhFVTzALoTbPqFhg29CShMT9kWkX5JbnCFQNg - oI - BiXWqYKlrHUcUfRzkCGNDsQ