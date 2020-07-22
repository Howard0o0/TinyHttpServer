#ifndef TINYTHREADPOOL_LOCKFREEQUE_H
#define TINYTHREADPOOL_LOCKFREEQUE_H

#include <atomic>
#include <iostream>
#include <semaphore.h>

namespace nethelper {

template < typename T >
struct ListNode {
	T	      val;
	ListNode< T >* next;
	ListNode< T >() : next(nullptr) {
	}
	ListNode< T >(T t) : val(t), next(nullptr) {
	}
};

template < typename T >
class LockFreeQue {
    public:
	LockFreeQue();
	void Push(T t);
	void PushInThread(T t);
	T    Pop();
	T    PopInThread();
	T    PopInThread(int sec);
	bool IsEmpty();

    private:
	ListNode< T >* head_;
	ListNode< T >* tail_;
	sem_t	  not_empty_;
};

template < typename T >
LockFreeQue< T >::LockFreeQue() : head_(nullptr), tail_(nullptr) {
	head_ = new ListNode< T >;
	tail_ = head_;
	sem_init(&not_empty_, 0, 0);
}

template < typename T >
void LockFreeQue< T >::Push(T t) {
	ListNode< T >* node = new ListNode< T >(t);

	/* insert a new node */
	while (!__sync_bool_compare_and_swap(&tail_->next, nullptr, node))
		;
	/* move tail_ to next node */
	while (!__sync_bool_compare_and_swap(&tail_, tail_, node))
		;
}
template < typename T >
void LockFreeQue< T >::PushInThread(T t) {
	ListNode< T >* node = new ListNode< T >(t);

	/* insert a new node */
	while (!__sync_bool_compare_and_swap(&tail_->next, nullptr, node))
		;
	/* move tail_ to next node */
	while (!__sync_bool_compare_and_swap(&tail_, tail_, node))
		;
	if (head_->next == tail_) {
		sem_post(&not_empty_);
		// std::cout << "notify not empty" << std::endl;
	}
}

template < typename T >
T LockFreeQue< T >::Pop() {
	ListNode< T >* p = nullptr;
	do {
		p = head_;
		if (!p->next) {
			return 0;
		}
	} while (!__sync_bool_compare_and_swap(&head_, p, p->next));
	T val = p->next->val;
	delete (p);
	return val;
}

template < typename T >
T LockFreeQue< T >::PopInThread() {
	ListNode< T >* p = nullptr;
	do {
		p = head_;
		while (!p->next) {
			// std::cout << "wait until not empty" << std::endl;
			sem_wait(&not_empty_);
			p = head_;
		}
	} while (!__sync_bool_compare_and_swap(&head_, p, p->next));
	T val = p->next->val;
	delete (p);
	return val;
}

template < typename T >
T LockFreeQue< T >::PopInThread(int sec) {
	ListNode< T >* p = nullptr;
	do {
		p = head_;
		if (!p->next) {
			// std::cout << "wait until not empty" << std::endl;
			struct timespec tm;
			tm.tv_sec = sec;
			sem_timedwait(&not_empty_, &tm);

			if (!p->next)
				return 0;
		}
	} while (!__sync_bool_compare_and_swap(&head_, p, p->next));

	T val = p->next->val;
	delete (p);
	return val;
}
template < typename T >
bool LockFreeQue< T >::IsEmpty() {
	return head_ == tail_;
}

}  // namespace nethelper

#endif