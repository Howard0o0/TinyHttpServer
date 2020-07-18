#ifndef TINYHTTPSERVER_WORKER_H
#define TINYHTTPSERVER_WORKER_H
#include "lockfreethreadpool.h"
#include "threadpool.h"
#include <functional>

using namespace tinythreadpool;

namespace ths {

typedef std::function< void(int sockfd, const std::string& msg) > OnMsgCallback;

class Worker {
    public:
	Worker(const OnMsgCallback& cb, int thread_cnt = 4);
	void HandleResponse(int client_fd);
	void HandleClientFd(int client_fd, int weak_thread_id);
	void SetOnMessageCallback(const OnMsgCallback& cb);

    private:
	// ThreadPool	 threadpool_;
	LockFreeQue< int > fd_translator_;
	int		   wakeup_fd_;
	OnMsgCallback      on_msg_cb_;
	int		   thread_cnt_;
	std::vector< int > epollfds_;
	std::mutex	 epollfds_lock_;
	std::vector< int > expired_fds;

	void	WorkFunc();
	std::string ReadMsg(int client_fd);
	void	OnMsgArrived(int sockfd, std::string msg);
};

}  // namespace ths

#endif