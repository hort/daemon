#ifndef WORKER_HPP_
#define WORKER_HPP_

#include "hort/chrono.hpp"

#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace hort
{

namespace detail {

/// \brief Thread safe queue
template <typename T>
class Tasks : public std::queue<T> {
	using callback_t = T;

	std::mutex mutex;

public:
	callback_t pop() {
		std::lock_guard<std::mutex> lock{mutex};
		callback_t callback = std::queue<callback_t>::front();
		std::queue<callback_t>::pop();
		return callback;
	}

	void push(const callback_t &callback) {
		std::lock_guard<std::mutex> lock{mutex};
		std::queue<callback_t>::push(callback);
	}

};

}

class Worker {
	using callback_t = std::function<void(void)>;

	detail::Tasks<callback_t> queue;
	std::vector<std::thread> threads;

	bool close_request;

	int tasks_total;
	int tasks_completed;

public:
	Worker()
		: queue{}
		, threads{1}
		, close_request{false}
		, tasks_total{0}
		, tasks_completed{0}
	{
		auto t = [this]() {
			while (!close_request || queue.size()) {
				if (queue.size()) {
					callback_t callback = queue.pop();
					callback();
					tasks_completed += 1;
				}
			}
		};

		for (auto &thread : threads)
			thread = std::thread(t);
	}

	~Worker()
	{
		close_request = true;
		for (auto &thread : threads)
			thread.join();
	}

	void close()
	{
		close_request = true;
	}

	void push(const callback_t &callback) {
		queue.push(callback);
		tasks_total += 1;
	}

	void progress()
	{
		const char *PBSTR   = "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||";
		const int   PBWIDTH = 60;

		while (queue.size()) {
			float percentage = std::clamp((float)tasks_completed / tasks_total, (float)0, (float)100);
			int val  = (int)(percentage * 100);
			int lpad = (int)(percentage * PBWIDTH);
			int rpad = PBWIDTH - lpad;
			printf("\r%3d%% [%.*s%*s] %d/%d", val, lpad, PBSTR, rpad, "", tasks_completed, tasks_total);
			fflush(stdout);
			hort::chrono::sleep(100);
		}
	}

};

} // namespace hort

#endif // WORKER_HPP_
