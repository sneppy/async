#pragma once

#include <utility>

#include "critical_section.h"
#include "event.h"
#include "count_ref.h"

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
	CountRefPtr<T> result;

public:
	/**
	 * Default constructor
	 */
	inline FutureBase()
		: event{nullptr}
		, result{}
	{
		//
	}

	/**
	 * Deleted copy constructor
	 */
	FutureBase(const FutureBase & other)
		: event{other.event}
		, result{other.result}
	{

	}

	/**
	 * Move constructor
	 */
	FutureBase(FutureBase && other)
		: event{other.event}
		, result{std::move(other.result)}
	{
		other.event = nullptr;
	}

	/**
	 * Deleted copy assignment
	 */
	FutureBase & operator=(const FutureBase & other)
	{
		event = other.event;
		result = other.result;
	}

	/**
	 * Move assignment
	 */
	FutureBase & operator=(FutureBase && other)
	{
		event = other.event;
		result = std::move(other.result);
		
		other.event = nullptr;
	}

	/**
	 * Init future
	 */
	void init()
	{
		// Init resources
		event = new Event;
		result = new RefPtr<T>{};
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
		result->set(std::forward<TT>(inT));

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
		do event->wait(); while (!result->isValid());

		// Move result out
		outT = result->get();

		// * Right now always returns true
		return true;
	}

	/**
	 * Cast to result
	 * 
	 * @return result
	 */
	inline operator T()
	{
		T future;
		return get(future) ? future : T{};
	}
};