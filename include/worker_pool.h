#pragma once

#include "critical_section.h"
#include "event.h"
#include "buffer.h"

#include <vector>
#include <functional>

/// Forward declare worker class
class Worker;

/**
 * A dynamically allocated pool of
 * worker threads
 */
class WorkerPool
{
	friend Worker;

protected:
	/// Jobs buffer
	Buffer<std::function<void()>> jobs;

	/// Worker list
	std::vector<Worker*> workers;

	/// Mutex
	pthread_mutex_t mutex;

	/// Has job event
	Event * event;

public:
	/**
	 * Init resource
	 */
	inline WorkerPool()
	{
		// Create threading controls
		pthread_mutex_init(&mutex, nullptr);
		event = new Event;
	}

	/**
	 * Release resources
	 */
	inline ~WorkerPool()
	{
		// Release threading controls
		pthread_mutex_destroy(&mutex);
		delete event;
	}

	/**
	 * Get num workers
	 */
	inline unsigned int getNumWorkers() const
	{
		return workers.size();
	}

	/**
	 * Set num threads
	 */
	void setNumWorkers(unsigned int inNumThreads);

	/**
	 * 
	 */
	void launch();

	/**
	 * Returns true if has pending jobs
	 */
	inline bool hasPendingJob() const
	{
		return !jobs.isEmpty();
	}

	/**
	 * Add to waiting list
	 */
	inline void waitForJob(Worker * worker)
	{
		// Wait for event to fire
		event->wait();
	}

	/**
	 * Returns pending job
	 * 
	 * @param [out] outJob returned job
	 * @return false if there is no pending job, true otherwise
	 */
	inline bool getPendingJob(std::function<void()> & outJob)
	{
		ScopeLock _{&mutex};
		return jobs.popFront(outJob);
	}

	/**
	 * Submit new job
	 * 
	 * @param [in] inJob submitted job
	 * @return false if buffer overflow, true otherwise
	 */
	bool submitJob(std::function<void()> && inJob);
};