#pragma once

#include <utility>

/**
 * Thread-safe circular buffer with
 * statically allocated storage
 */
template<typename T, size_t size = 2048>
class Buffer
{
protected:
	/// Static size buffer
	T buffer[size];

	/// Buffer head pointer
	size_t headp;

	/// Buffer tail pointer
	size_t tailp;

	/// Resource level mutex
	mutable pthread_mutex_t mutex;

public:
	/**
	 * Default constructor
	 */
	inline Buffer()
		: headp{0}
		, tailp{0}
	{
		// Init mutex
		pthread_mutex_init(&mutex, nullptr);
	}

	/**
	 * Destructor
	 */
	~Buffer()
	{
		// Destroy elements
		for (unsigned int it = headp; it != tailp; it = ++it == size ? 0 : it)
			buffer[it].~T();
		
		// Release mutex
		pthread_mutex_destroy(&mutex);
	}

	/**
	 * Returns true if buffer is empty
	 */
	inline bool isEmpty() const
	{
		ScopeLock _{&mutex};
		return headp == tailp;
	}

	/**
	 * Push item to the back of the
	 * queue
	 * 
	 * @param [in] inT item to insert
	 * @return false if buffer is exhausted
	 */
	template<typename TT>
	bool pushBack(TT && inT)
	{
		ScopeLock _{&mutex};

		// Reserve space
		size_t new_tailp = (tailp + 1) == size ? 0 : (tailp + 1);
		if (new_tailp != headp)
		{
			// Move in and increment tail pointer
			buffer[tailp] = std::forward<TT>(inT);
			tailp = new_tailp;

			return true;
		}
		else return false;
	}

	/**
	 * Removes the first item in queue and
	 * moves it in the parameter
	 * 
	 * @param [out] outT moved item
	 * @return true if buffer is not empty
	 */
	bool popFront(T & outT)
	{
		ScopeLock _{&mutex};

		if (headp != tailp)
		{
			// Move out and increment head pointer
			outT = std::move(buffer[headp]);
			headp = ++headp == size ? 0 : headp;

			return true;
		}
		else return false;
	}
};
