#include "threadpool.h"
#include "threadpooltest.h"
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

	Test1();
	Test2();
	Test3();
	Test4();

	return 0;
}
