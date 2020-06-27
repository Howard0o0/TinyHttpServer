#include "maintest.h"
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>

std::mutex		mu;
std::condition_variable cond;

void Product() {
	while (1) {
		cond.notify_all();
		sleep(1);
	}
}

void Consume1() {
	while (1) {
		{
			std::unique_lock< std::mutex > locker(mu);
			cond.wait(locker);
		}
		std::cout << "consume1" << std::endl;
		sleep(2);
	}
}
void Consume2() {
	while (1) {
		{
			std::unique_lock< std::mutex > locker(mu);
			cond.wait(locker);
		}
		std::cout << "consume2" << std::endl;
		sleep(2);
	}
}
void Consume3() {
	while (1) {
		{
			std::unique_lock< std::mutex > locker(mu);
			cond.wait(locker);
		}
		std::cout << "consume3" << std::endl;
		sleep(2);
	}
}

void CondTest() {
	std::thread t1(std::bind(Product));
	std::thread t2(std::bind(Consume1));
	std::thread t3(std::bind(Consume2));
	std::thread t4(std::bind(Consume3));

	t1.join();
	t2.join();
	t3.join();
	t4.join();
}