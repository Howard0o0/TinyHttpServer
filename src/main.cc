#include "tcpserver.h"
#include "test.h"
#include "threadpool.h"
#include <iostream>
#include <semaphore.h>
#include <string>
#include <unistd.h>

void OnMsgCb(int fd, const std::string& msg) {
	printf("new msg : \n %s \n", msg.data());
}

int main() {

	// Test1();
	// Test2();
	// Test3();
	// Test4();
	// Test6();
	// TestLockFreeThreadPool();

	ths::TcpServer tcpserver(10000);
	tcpserver.SetOnMsgCallback(std::bind(OnMsgCb, std::placeholders::_1,
					     std::placeholders::_2));
	tcpserver.Start();
	// tcpserver.SingleLoop();

	return 0;
}
