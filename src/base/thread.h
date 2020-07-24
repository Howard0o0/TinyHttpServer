#ifndef NETHELPER_THREAD_H
#define NETHELPER_THREAD_H

#include "callback.h"
#include "noncopyable.h"
#include <assert.h>
#include <functional>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

namespace nethelper {

class Thread : Noncopyable {
    public:
	explicit Thread(Task task) : task_(task), is_running_(false) {
		Task* taskptr = new Task(task_);
		assert(!pthread_create(&tid_, NULL, ThreadFunc,
				       reinterpret_cast< void* >(taskptr)));
		is_running_ = true;
	}
	~Thread() {
		if (is_running_)
			assert(!pthread_detach(tid_));
	}
	void Join() {
		assert(is_running_);
		assert(!pthread_join(tid_, NULL));
		is_running_ = false;
	}
	static int GetThreadid() {
		return syscall(SYS_gettid);
	}

    private:
	Task	  task_;
	pthread_t tid_;
	bool	  is_running_;

    private:
	static void* ThreadFunc(void* arg) {
		Task* taskptr = reinterpret_cast< Task* >(arg);
		Task  task    = *taskptr;
		delete (taskptr);
		task();
		return NULL;
	}
};

}  // namespace nethelper

#endif