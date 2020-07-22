#ifndef TINYTHREADPOOL_LOCKFREETHREADPOOL_H
#define TINYTHREADPOOL_LOCKFREETHREADPOOL_H

#include "lockfreeque.h"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <semaphore.h>
#include <thread>
#include <vector>

namespace nethelper {

class LockFreeThreadPool {

    public:
	typedef std::function< void() > Task;

	LockFreeThreadPool();
	~LockFreeThreadPool();
	void RunTask(Task task);
	void Start(unsigned int size);
	void Stop();

	static void RunTaskInGlobalThreadPool(Task task);

    private:
	std::vector< std::unique_ptr< std::thread > > threads_;
	LockFreeQue< Task >			      tasks_;
	std::atomic_bool			      running_;
	std::atomic_bool			      wait_tasks_empty_;
	sem_t					      tasks_empty_;

	void			 ConsumeTask();
	bool			 IsRunning() const;
	LockFreeThreadPool::Task FetchTask();
	void			 WaitAllTasksDone();
};

}  // namespace nethelper

#endif