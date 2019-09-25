#pragma once

#include <pthread.h>

/**
 * Helper function to lock a section of code
 * When an instance of scope lock is created
 * the code inside its scope is locked w.r.t.
 * multi-thread access
 */
class ScopeLock
{
protected:
	/// Reference to mutex
	pthread_mutex_t * mutex;

public:
	/**
	 * Acquire lock
	 */
	explicit inline ScopeLock(pthread_mutex_t * inMutex)
		: mutex{inMutex}
	{
		// Lock mutex
		pthread_mutex_lock(mutex);
	}

	/**
	 * Release lock
	 */
	inline ~ScopeLock()
	{
		pthread_mutex_unlock(mutex);
	}
};