#include "test.h"
#include "hiredis/hiredis.h"
#include "httprequest.h"
#include "lockfreeque.h"
#include "lockfreethreadpool.h"
#include "log.h"
#include "redistool.h"
#include "threadpool.h"
#include <boost/locale/encoding.hpp>
#include <codecvt>
#include <condition_variable>
#include <fstream>
#include <functional>
#include <iostream>
#include <locale>
#include <mutex>
#include <re2/re2.h>
#include <string>
#include <thread>
#include <unistd.h>

void PrintWithoutArg() {
	std::cout << "print without args" << std::endl;
}

void PrintWithArg(const std::string& str) {
	std::cout << "print with arg: " << str << std::endl;
}

class Printer {
    public:
	void PrintWithArg(const std::string& str) {
		std::cout << "print with arg by printer: " << str << std::endl;
	}
};

void Test1() {
	std::cout << "==========run function without arg" << std::endl;

	nethelper::ThreadPool threadpool;
	threadpool.Start(2);
	threadpool.RunTask(std::bind(PrintWithoutArg));
}

void Test2() {
	std::cout << "==========run function with arg============="
		  << std::endl;

	nethelper::ThreadPool threadpool;
	threadpool.Start(2);
	threadpool.RunTask(std::bind(PrintWithArg, "hello world!"));
}
void Test3() {
	std::cout << "==========run object's function with arg============="
		  << std::endl;
	nethelper::ThreadPool threadpool;
	threadpool.Start(2);

	Printer printer;
	threadpool.RunTask(
		std::bind(&Printer::PrintWithArg, &printer, "hello world!"));
}
void Test4() {

	std::cout << "==========run object's function with arg in global "
		     "pool============="
		  << std::endl;
	Printer printer;
	nethelper::ThreadPool::RunTaskInGlobalThreadPool(
		std::bind(&Printer::PrintWithArg, &printer, "hello world!"));
}

nethelper::LockFreeQue< int > que;

void LockFreePush() {
	for (int j = 0; j < 10; j++) {
		que.Push(j);
		sleep(1);
	}
}
void LockFreePop() {

	while (1) {
		int i = que.Pop();
		if (i)
			std::cout << i << std::endl;
	}
}
int  cnt = 0;
void LockFreePushLoop() {
	while (1) {
		que.PushInThread(1);
		sleep(1);
	}
}

void LockFreePopLoop() {
	while (1) {
		int i = que.PopInThread();

		std::cout << i << std::endl;
	}
}

void Test5() {
	std::thread t1(std::bind(LockFreePush));
	std::thread t2(std::bind(LockFreePop));
	std::thread t3(std::bind(LockFreePop));
	t1.join();
	t2.join();
	t3.join();
}
void Test6() {
	std::thread t1(std::bind(LockFreePushLoop));
	std::thread t4(std::bind(LockFreePushLoop));
	std::thread t2(std::bind(LockFreePopLoop));
	std::thread t3(std::bind(LockFreePopLoop));
	std::thread t5(std::bind(LockFreePopLoop));
	std::thread t6(std::bind(LockFreePopLoop));
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
}

void Test7() {
}
void TestLockFreeThreadPool() {
	std::cout << "==========run object's function with arg============="
		  << std::endl;
	nethelper::LockFreeThreadPool threadpool;
	threadpool.Start(2);

	Printer printer;
	for (int i = 0; i < 10; i++)
		threadpool.RunTask(
			std::bind(&Printer::PrintWithArg, &printer,
				  "print" + std::to_string(i) + "!"));
}

std::wstring to_wstring(const std::string& src) {
	//   std::setlocale(LC_CTYPE, "");
	std::setlocale(LC_CTYPE, "zh_CN");
	std::wstring	       dest;
	size_t const	       wcs_len = mbstowcs(NULL, src.c_str(), 0);
	std::vector< wchar_t > tmp(wcs_len + 1);
	mbstowcs(&tmp[ 0 ], src.c_str(), src.size());

	dest.assign(tmp.begin(), tmp.end() - 1);
	printf("hh\n");

	return dest;
}

std::wstring s2ws(const std::string& s) {
	const char* str	 = s.c_str();
	size_t	    len	 = s.size() + 1;
	wchar_t*    wstr = new wchar_t[ len ];
	std::mbstowcs(wstr, str, len);
	std::wstring ret(wstr);
	delete[] wstr;
	return ret;
}
void LinuxCommandTest() {
	HttpRequest request;
	request.clearHeader();
	request.appendHeader("Host:man.he.net");
	request.appendHeader("Cache-Control: no-cache");
	request.appendHeader("Accept: "
			     "text/html,application/xhtml+xml,application/"
			     "xml;q=0.9,image/webp,image/apng,*/"
			     "*;q=0.8,application/signed-exchange;v=b3;q=0.9");
	request.appendHeader("Accept-Encoding: gzip, deflate, br");

	long status_code =
		request.get("http://man.he.net/?topic=ab&section=all");
	printf("%ld\n", status_code);
	if (status_code == 200) {
		std::string content = request.getResponse();
		RE2::GlobalReplace(&content, "<.*?>", "");
		// std::ofstream ofs("ab.md", std::ios::out);
		// ofs << content;
		std::cout << content << std::endl;
		// ofs.close();
	}
}

void HiredisTest() {

	RedisTool redistool;
	redistool.ConnectDatabase("127.0.0.1", 6379);
	std::string value = redistool.GetString("foo");
	std::cout << value << std::endl;
	std::cout << "curr tid:" << nethelper::Thread::GetThreadid()
		  << std::endl;

	// redisContext* conn = redisConnect("127.0.0.1", 6379);
	// if (conn->err)
	// 	printf("connection error:%s\n", conn->errstr);

	// redisReply* reply = ( redisReply* )redisCommand(conn, "set foo
	// 1234"); freeReplyObject(reply);

	// reply = ( redisReply* )redisCommand(conn, "get foo");

	// printf("%s\n", reply->str);
	// freeReplyObject(reply);

	// redisFree(conn);

	// RedisTool redis_tool;
	// if (!redis_tool.ConnectDatabase("127.0.0.1", 6379))
	// 	return;

	// std::cout << redis_tool.GetString("k1") << std::endl;
}

void HiredisTest2() {
	nethelper::LockFreeThreadPool threadpool;
	threadpool.Start(4);
	for (int i = 0; i < 100; ++i)
		threadpool.RunTask(HiredisTest);
}