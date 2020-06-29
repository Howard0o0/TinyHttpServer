
#include "lockfreethreadpool.h"
#include <functional>
#include <iostream>
#include <thread>

using namespace tinythreadpool;

/* pubic */

LockFreeThreadPool::LockFreeThreadPool() {
	wait_tasks_empty_ = false;
	sem_init(&tasks_empty_, 0, 1);
}
void LockFreeThreadPool::Start(unsigned int size) {
	running_ = true;

	for (decltype(size) i = 0; i < size; i++) {
		threads_.emplace_back(new std::thread(
			&LockFreeThreadPool::ConsumeTask, this));
	}
}
LockFreeThreadPool::~LockFreeThreadPool() {
	if (IsRunning())
		Stop();
}
void LockFreeThreadPool::RunTask(Task task) {

	tasks_.PushInThread(std::move(task));
}
void LockFreeThreadPool::Stop() {

	WaitAllTasksDone();

	running_ = false;

	for (auto& thrd : threads_)
		thrd->join();
}
void LockFreeThreadPool::RunTaskInGlobalThreadPool(Task task) {
	static bool		  global_thread_pool_existed = false;
	static LockFreeThreadPool global_thread_pool;
	if (!global_thread_pool_existed) {
		global_thread_pool.Start(std::thread::hardware_concurrency());
		global_thread_pool_existed = true;
	}

	global_thread_pool.RunTask(std::move(task));
}
/* end of pubic */

/* private */

void LockFreeThreadPool::WaitAllTasksDone() {
	wait_tasks_empty_ = true;
	while (!tasks_.IsEmpty()) {
		sem_wait(&tasks_empty_);
	}
	// std::cout << "tasks done" << std::endl;
}

LockFreeThreadPool::Task LockFreeThreadPool::FetchTask() {

	if (wait_tasks_empty_.load() && tasks_.IsEmpty())
		sem_post(&tasks_empty_);
	return tasks_.PopInThread(1);
}

void LockFreeThreadPool::ConsumeTask() {
	while (IsRunning()) {

		Task task(FetchTask());
		if (task)
			task();
		// std::cout << "consuming" << std::endl;
	}
}

bool LockFreeThreadPool::IsRunning() const {

	return running_.load();
}
/* end of private */
