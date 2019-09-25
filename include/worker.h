#pragma once

#include "critical_section.h"
#include "event.h"

/// Forward declare worker pool
class WorkerPool;

/**
 * A single worker, part of a worker pool
 */
class Worker
{
	friend WorkerPool;

protected:
	/// Worker id
	const unsigned int id;

	/// Reference to pool
	WorkerPool * owner;

	/// Reference to owned threading resource
	pthread_t * thread;

	/// Requesting shutdown flag
	bool bRequestShutdown;

public:
	/**
	 * Default constructor
	 */
	explicit inline Worker(unsigned int inId, WorkerPool * inOwner)
		: id{inId}
		, owner{inOwner}
		, thread{nullptr}
		, bRequestShutdown{false}
	{
		//
	}

	/**
	 * Destructor
	 */
	inline ~Worker()
	{
		if (thread)
		{
			// Graceful exit
			bRequestShutdown = true;
			pthread_join(*thread, nullptr);
		}
	}

	/**
	 * Start worker
	 */
	void start();

	/**
	 * Init worker
	 */
	inline void init()
	{
		//
	}

	/**
	 * Worker loop
	 */
	void run();

	/**
	 * Shutdown worker
	 */
	inline void shutdown()
	{
		//
	}

protected:
	/**
	 * Static thread entry function
	 */
	static void * launchWorkerThread(void * worker);
};