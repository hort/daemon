#ifndef WORKER_HPP_
#define WORKER_HPP_

#include <functional>
#include <queue>
#include <mutex>
#include <thread>

#include "hort/interface.hpp"

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

namespace hort
{

namespace detail {

/// \brief Thread safe task queue
class Tasks : public std::queue<std::function<void(void)>> {
	using callback_t = std::function<void(void)>;

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

class Worker : public detail::Tasks {

	using callback_t = std::function<void(void)>;

	std::queue<callback_t> queue;
	std::vector<std::thread> threads;

	bool close_request;

	int tasks_total;
	int tasks_completed;

public:
	Worker()
		: queue{}
		, threads{5}
		, close_request{false}
		, tasks_total{0}
		, tasks_completed{0}
	{
		auto t = [this]() {
			while (!close_request || size()) {
				if (size()) {
					callback_t callback = pop();
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
		detail::Tasks::push(callback);

		tasks_total += 1;
	}

	void progress()
	{
		while(size()) {
			float percentage = clamp((float)tasks_completed / tasks_total, 0, 100);
			int val  = (int)(percentage * 100);
			int lpad = (int)(percentage * PBWIDTH);
			int rpad = PBWIDTH - lpad;
			printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
			fflush(stdout);
			sleep(100);
		}
	}

};

} // namespace hort

#endif // WORKER_HPP_
