#ifndef TINYTHREADPOOL_THREADPOOL_H
#define TINYTHREADPOOL_THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace tinythreadpool {

class ThreadPool {

    public:
	typedef std::function< void() > Task;

	~ThreadPool();
	void RunTask(Task task);
	void Start(unsigned int size);
	void Stop();

	static void RunTaskInGlobalThreadPool(Task task);

    private:
	std::vector< std::unique_ptr< std::thread > > threads_;
	// std::vector<unsigned long long > threadids_;
	// std::mutex		threadids_lock_;
	std::queue< Task >      tasks_;
	bool			running_;
	std::mutex		tasks_lock_;
	std::condition_variable tasks_cond_;
	std::mutex		running_lock_;
	std::mutex		tasks_empty_;
	std::condition_variable tasks_empty_cond_;

	void		 ConsumeTask();
	bool		 IsRunning();
	ThreadPool::Task FetchTask();
	void		 NotifyStop();
	void		 WaitAllTasksDone();
};

}  // namespace tinythreadpool
#endif