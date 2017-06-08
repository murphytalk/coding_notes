#ifndef BLOCKINGQ_H
#define BLOCKINGQ_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <cassert>

namespace Utils {
	template<typename T>
	class blocking_queue {
	private:
		std::mutex _mutex;
		std::condition_variable _cond;
		std::queue<T> _q;
	public:
		T pop() {
			std::unique_lock<std::mutex> lock(_mutex);
			//only exit wait when queue is empty, to prevent spurious wakeup
			_cond.wait(lock, [this] {return !_q.empty(); });
			assert(!_q.empty());
			auto data = std::move(_q.front());
			_q.pop();
			return data;
		}

		void push(const T& data) {
			std::unique_lock<std::mutex> lock(_mutex);
			_q.push(std::move(data));
			lock.unlock();
			_cond.notify_one();
		}
	};
}

#endif
