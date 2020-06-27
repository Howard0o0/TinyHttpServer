#ifndef TINYTHREADPOOL_THREADPOOL_H
#define TINYTHREADPOOL_THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {

    public:
	ThreadPool(unsigned int size);

	typedef std::function< void() > Task;

    private:
	std::queue< std::unique_ptr< std::thread > > threads_;
	std::queue< Task >			     tasks_;
	bool					     running_;
	std::mutex				     lock_;
	std::condition_variable			     cond_;
	std::mutex				     running_lock_;

	void ConsumeTask();
	bool IsRunning();
};


#endif