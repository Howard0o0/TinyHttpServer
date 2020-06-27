#include "threadpool.h"
#include <functional>
#include <thread>

/* pubic */
ThreadPool::ThreadPool(unsigned int size) : running_(true) {

	for (decltype(size) i = 0; i < size; i++) {
		threads_.emplace(new std::thread(&ThreadPool::ConsumeTask,
						 std::ref(*this)));
	}
}
/* end of pubic */

/* private */

void ThreadPool::ConsumeTask() {
	while (IsRunning()) {
		std::unique_lock< std::mutex > locker(this->lock_);
		while (tasks_.empty())
			cond_.wait(locker);

		Task task = tasks_.front();
		tasks_.pop();
		locker.unlock();
		task();
	}
}

bool ThreadPool::IsRunning() {
	std::lock_guard< std::mutex > lock(this->running_lock_);
	return running_;
}
/* end of private */
