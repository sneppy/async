#pragma once

#include <utility>

#include "critical_section.h"
#include "event.h"

/**
 * 
 */
template<typename T>
class FutureBase
{
protected:
	/// Completion event
	Event * event;

	/// Future result
	T result;

	/// Has result flag
	bool bHasResult;

public:
	/**
	 * Default constructor
	 */
	inline FutureBase()
		: event{nullptr}
		, result{}
		, bHasResult{}
	{
		//
	}

	/**
	 * Deleted copy constructor
	 */
	FutureBase(const FutureBase & other) = delete;

	/**
	 * Move constructor
	 */
	FutureBase(FutureBase && other)
		: event{other.event}
		, bHasResult{other.bHasResult}
	{
		if (other.bHasResult)
			result = std::move(other.result);
		
		other.event = nullptr;
		other.bHasResult = false;
	}

	/**
	 * Deleted copy assignment
	 */
	FutureBase & operator=(const FutureBase & other) = delete;

	/**
	 * Move assignment
	 */
	FutureBase & operator=(FutureBase && other)
	{
		event = other.event;
		if ((bHasResult = other.bHasResult))
			result = std::move(other.result);
		
		other.event = nullptr;
		other.bHasResult = false;
	}

	/**
	 * Init future
	 */
	void init()
	{
		// Init resources
		event = new Event;
	}

	/**
	 * Destroy future
	 */
	void destroy()
	{
		// Release resources
		if (event) delete event;
	}

	/**
	 * Destructor
	 */
	inline ~FutureBase()
	{
		destroy();
	}

	/**
	 * Set future result
	 */
	template<typename TT>
	void set(TT && inT)
	{
		// Set result value and flag
		result = std::forward<TT>(inT);
		bHasResult = true;

		// Signal completion
		event->fire<Event::TRIGGER_ALL>();
	}

	/**
	 * Waits for the result to be
	 * available and returns it
	 * 
	 * @param [in] outT future result
	 * @return 
	 */
	bool get(T & outT)
	{
		// Wait until result is available
		do event->wait(); while (!bHasResult);

		// Move result out
		outT = std::move(result);

		// * Right now always returns true
		return true;
	}
};