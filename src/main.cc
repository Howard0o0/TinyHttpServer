#include "test.h"
#include "threadpool.h"
#include <iostream>
#include <semaphore.h>
#include <string>
#include <unistd.h>

sem_t sem;

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

void Productor() {
	while (1) {
		sem_post(&sem);
		sleep(2);
	}
}

void Consum1() {
	while (1) {
		sem_wait(&sem);
		printf("consum1 \r\n");
	}
}
void Consum2() {
	while (1) {
		sem_wait(&sem);
		printf("consum2 \r\n");
	}
}

int main() {

	// Test1();
	// Test2();
	// Test3();
	// Test4();
	// Test6();
	TestLockFreeThreadPool();

	// sem_init(&sem, 0, 0);
	// std::thread t1(std::bind(Consum1));
	// std::thread t2(std::bind(Consum2));
	// std::thread t3(std::bind(Productor));

	// t1.join();
	// t2.join();
	// t3.join();

	return 0;
}
