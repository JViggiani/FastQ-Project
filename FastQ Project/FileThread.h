#pragma once

#include "pch.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <chrono>

class ThreadPool
{
public:

	ThreadPool(int threads) : shutdown_(false)
	{
		// Create the specified number of threads
		threads_.reserve(threads);
		for (int i = 0; i < threads; ++i)
			threads_.emplace_back(std::bind(&ThreadPool::threadEntry, this, i));
	}

	~ThreadPool()
	{
		{
			// Unblock any threads and tell them to stop
			std::unique_lock <std::mutex> l(lock_);

			shutdown_ = true;
			condVar_.notify_all();
		}

		// Wait for all threads to stop
		std::this_thread::sleep_for(std::chrono::seconds(1));
		//std::cout << "Joining threads" << "\n";
		for (auto& thread : threads_)
			thread.join();
	}

	void doJob(std::function <void(void)> func)
	{
		// Place a job on the queu and unblock a thread
		std::unique_lock <std::mutex> l(lock_);

		jobs_.emplace(std::move(func));
		condVar_.notify_one();
	}

protected:

	void threadEntry(int i)
	{
		std::function <void(void)> job;

		while (1)
		{
			{
				std::unique_lock <std::mutex> l(lock_);

				while (!shutdown_ && jobs_.empty())
					condVar_.wait(l);

				if (jobs_.empty())
				{
					// No jobs to do and we are shutting down
					std::this_thread::sleep_for(std::chrono::seconds(1));
					//std::cout << "Thread " << i << " terminates" << "\n";
					return;
				}

				std::this_thread::sleep_for(std::chrono::seconds(1));
				//std::cout << "Thread " << i << " does a job" << "\n";
				job = std::move(jobs_.front());
				jobs_.pop();
			}

			// Do the job without holding any locks
			job();
		}

	}

	std::mutex lock_;
	std::condition_variable condVar_;
	bool shutdown_;
	std::queue <std::function <void(void)>> jobs_;
	std::vector <std::thread> threads_;
};

/*
class FileThreadPool
{
public:
	FileThreadPool(size_t aMaxThreads)
	{
		_maxThreads = aMaxThreads;
		// Create the specified number of threads
		_threadPool.reserve(threads);
		for (int i = 0; i < threads; ++i)
			threads_.emplace_back(std::bind(&ThreadPool::threadEntry, this, i));
	}

	vector<FileThread>& getThreadPool()
	{
		return _threadPool;
	}

	size_t getMaxThreads()
	{
		return _maxThreads;
	}

	void setMaxThreads(const size_t& aNewMax)
	{
		_maxThreads = aNewMax;
	}

	void joinAllThreads()
	{
		for (auto& th : _threadPool) 
			th._thread.join();
	}
private:
	int _maxThreads;
	vector<FileThread> _threadPool;
};
*/