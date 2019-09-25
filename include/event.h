#pragma once

#include "critical_section.h"

/**
 * Generic class event
 */
class Event
{
public:
	/**
	 * Event state
	 */
	enum State : unsigned char
	{
		TRIGGER_NONE,
		TRIGGER_ONE,
		TRIGGER_ALL
	};

protected:
	/// Pthread sync variable
	pthread_cond_t cond;

	/// Pthread mutex
	pthread_mutex_t mutex;

	/// Clients waiting in line
	unsigned int numWaiting;

	/// Event state
	State state;

public:
	/**
	 * Default constructor
	 */
	inline Event()
		: numWaiting{0}
		, state{TRIGGER_NONE}
	{
		// Init threading resources
		pthread_cond_init(&cond, nullptr);
		pthread_mutex_init(&mutex, nullptr);
	}

	/**
	 * Destructor
	 */
	inline ~Event()
	{
		// Release threading resources
		pthread_cond_destroy(&cond);
		pthread_mutex_destroy(&mutex);
	}
	
	/**
	 * Wait for thread
	 */
	bool wait();

	/**
	 * Trigger event
	 * 
	 * @param [in] type fire type (either State::TRIGGER_ONE or State::TRIGGER_ALL)
	 */
	template<State type = State::TRIGGER_ONE>
	void fire()
	{
		ScopeLock _{&mutex};

		switch ((state = type))
		{
		case State::TRIGGER_ONE:
			pthread_cond_signal(&cond);
			break;

		case State::TRIGGER_ALL:
			pthread_cond_broadcast(&cond);
			break;

		default:
			// Useless
			break;
		}
	}
};