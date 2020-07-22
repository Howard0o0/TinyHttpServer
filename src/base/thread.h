#ifndef NETHELPER_THREAD_H
#define NETHELPER_THREAD_H

#include "callback.h"
#include <functional>
#include <pthread.h>

namespace nethelper {

class Thread {
    public:
	explicit Thread(Task task) : task_(task) {
		Task* taskptr = new Task(task_);
		pthread_create(&tid_, NULL, ThreadFunc,
			       reinterpret_cast< void* >(taskptr));
	}

    private:
	Task      task_;
	pthread_t tid_;

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