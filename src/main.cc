#include "httprequest.h"
#include "httpserver.h"
#include "men.h"
#include "re2/re2.h"
#include "tcpserver.h"
#include "test.h"
#include "threadpool.h"
#include <iostream>
#include <semaphore.h>
#include <string>
#include <unistd.h>

int main() {

	// Test1();
	// Test2();
	// Test3();
	// Test4();
	// Test6();
	// TestLockFreeThreadPool();
	// HiredisTest2();

	// HttpServer httpserver(10000, 4);
	// httpserver.StartLoop();

	// char link[ 100 ];
	// char path[ 100 ];
	// sprintf(link, "/proc/%d/exe", getpid());
	// int i	  = readlink(link, path, sizeof(path));
	// path[ i ] = '\0';
	// printf("path:%s\n", path);

	Men men(10000, 4);
	men.StartLoop();

	// LogTest();

	return 0;
}
