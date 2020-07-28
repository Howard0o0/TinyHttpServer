#include "threadpool.h"
#include "thread.h"
#include <functional>
#include <iostream>
#include <thread>

using namespace nethelper;

/* pubic */

void ThreadPool::Start(unsigned int size) {
	running_ = true;

	for (decltype(size) i = 0; i < size; i++) {
		threads_.emplace_back(
			new Thread(std::bind(&ThreadPool::ConsumeTask, this)));
	}
}
ThreadPool::~ThreadPool() {
	if (IsRunning())
		Stop();
}
void ThreadPool::RunTask(Task task) {

	std::unique_lock< std::mutex > locker(this->tasks_lock_);
	tasks_.push(std::move(task));
	tasks_cond_.notify_one();
}
void ThreadPool::Stop() {
	WaitAllTasksDone();

	do {
		std::lock_guard< std::mutex > locker_running(
			this->running_lock_);
		running_ = false;
	} while (0);

	NotifyStop();

	for (auto& thrd : threads_)
		thrd->Join();
}
void ThreadPool::RunTaskInGlobalThreadPool(Task task) {
	static bool	  global_thread_pool_existed = false;
	static ThreadPool global_thread_pool;
	if (!global_thread_pool_existed) {
		global_thread_pool.Start(std::thread::hardware_concurrency());
		global_thread_pool_existed = true;
	}

	global_thread_pool.RunTask(std::move(task));
}
/* end of pubic */

/* private */

void ThreadPool::WaitAllTasksDone() {
	while (!tasks_.empty()) {
		std::unique_lock< std::mutex > locker(this->tasks_empty_);
		tasks_empty_cond_.wait(locker);
	}
	// std::cout << "tasks done" << std::endl;
}

void ThreadPool::NotifyStop() {
	tasks_cond_.notify_all();
}
ThreadPool::Task ThreadPool::FetchTask() {

	std::unique_lock< std::mutex > locker(this->tasks_lock_);
	while (IsRunning() && tasks_.empty())
		tasks_cond_.wait(locker);

	// std::cout << "got locker !" << std::this_thread::get_id() <<
	// std::endl;

	Task task;
	if (!tasks_.empty()) {
		task = tasks_.front();
		tasks_.pop();
	}

	if (tasks_.empty())
		tasks_empty_cond_.notify_all();

	return task;
}

void ThreadPool::ConsumeTask() {
	while (IsRunning()) {

		Task task(FetchTask());
		if (task)
			task();
	}
}

bool ThreadPool::IsRunning() {
	std::lock_guard< std::mutex > lock(this->running_lock_);

	return running_;
}
/* end of private */
