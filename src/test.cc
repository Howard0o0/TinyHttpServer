#include "test.h"
#include "lockfreeque.h"
#include "lockfreethreadpool.h"
#include "threadpool.h"
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
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