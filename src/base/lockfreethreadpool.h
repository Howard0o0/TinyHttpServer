#ifndef TINYTHREADPOOL_LOCKFREETHREADPOOL_H
#define TINYTHREADPOOL_LOCKFREETHREADPOOL_H

#include "callback.h"
#include "lockfreeque.h"
#include "thread.h"
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
	LockFreeThreadPool();
	~LockFreeThreadPool();
	void RunTask(Task task);
	void Start(unsigned int size);
	void Stop();

	static void RunTaskInGlobalThreadPool(Task task);

    private:
	std::vector< std::unique_ptr< Thread > > threads_;
	LockFreeQue< Task >			 tasks_;
	std::atomic_bool			 running_;
	std::atomic_bool			 wait_tasks_empty_;
	sem_t					 tasks_empty_;

	void ConsumeTask();
	bool IsRunning() const;
	Task FetchTask();
	void WaitAllTasksDone();
};

}  // namespace nethelper

#endif