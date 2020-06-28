#include "threadpool.h"
#include "threadpooltest.h"
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
	std::cout << "==========run function without arg============="
		  << std::endl;

	tinythreadpool::ThreadPool threadpool;
	threadpool.Start(2);
	threadpool.RunTask(std::bind(PrintWithoutArg));
}

void Test2() {
	std::cout << "==========run function with arg============="
		  << std::endl;

	tinythreadpool::ThreadPool threadpool;
	threadpool.Start(2);
	threadpool.RunTask(std::bind(PrintWithArg, "hello world!"));
}
void Test3() {
	std::cout << "==========run object's function with arg============="
		  << std::endl;
	tinythreadpool::ThreadPool threadpool;
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
	tinythreadpool::ThreadPool::RunTaskInGlobalThreadPool(
		std::bind(&Printer::PrintWithArg, &printer, "hello world!"));
}
