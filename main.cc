#include "maintest.h"
#include "threadpool.h"
#include <iostream>
#include <string>
#include <unistd.h>

void PrintStr(const std::string& str) {
	std::cout << str << std::endl;
}

void Print() {
	std::cout << "haha" << std::endl;
}

class A {
    public:
	void PrintStr(const std::string& str) {
		std::cout << str << std::endl;
		sleep(1);
	}
};

int main() {
	A	  a;
	ThreadPool thread_pool;
	thread_pool.Start(4);
	for (int i = 0; i < 12; i++)
		thread_pool.RunTask(bind(&A::PrintStr, &a, "xiha"));

	sleep(2);
	thread_pool.Stop();
	// CondTest();

	return 0;
}
