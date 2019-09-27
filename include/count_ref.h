#pragma once

#include "atomic.h"

/**
 * 
 */
template<typename T>
class RefPtr
{
protected:
	/// Pointed object
	T * obj;

	/// Reference count
	Atomic<unsigned int> refCount;

public:
	/**
	 * Default constructor
	 */
	inline RefPtr()
		: obj{nullptr}
		, refCount{0}
	{
		//
	}

	/**
	 * Initialize with value
	 */
	inline RefPtr(T * inObj)
		: obj{inObj}
		, refCount{0}
	{
		//
	}

	/**
	 * Returns true if pointer is valid
	 */
	inline bool isValid()
	{
		return obj != nullptr;
	}

	/**
	 * Get object reference
	 * @{
	 */
	inline T & get()
	{
		return *obj;
	}

	inline const T & get() const
	{
		return *obj;
	}
	/// @}

	/**
	 * Set object value
	 * 
	 * @param [in] value object value
	 */
	template<typename TT>
	inline RefPtr & set(TT && value)
	{
		if (obj)
		{
			*obj = std::forward<TT>(value);
		}
		else
		{
			obj = new T{std::forward<TT>(value)};
		}

		return *this;
	}

	/**
	 * Acquire reference
	 */
	inline void acquire()
	{
		++refCount;
	}

	/**
	 * Release reference
	 */
	inline void release()
	{
		--refCount;
	}
};

/**
 * Managed dynamic allocation
 */
template<typename T>
class CountRefPtr
{
protected:
	/// Pointer to object
	RefPtr<T> * ref;

public:
	/**
	 * Default constructor
	 */
	inline CountRefPtr()
		: ref{nullptr}
	{
		
	}

	/**
	 * Initialize with pointer
	 */
	inline CountRefPtr(RefPtr<T> * inRef)
		: ref{inRef}
	{
		if (ref) ref->acquire();
	}

	/**
	 * Copy constructor, increment ref counter
	 */
	inline CountRefPtr(const CountRefPtr & other)
		: ref{other.ref}
	{
		if (ref) ref->acquire();
	}

	/**
	 * Move constructor
	 */
	inline CountRefPtr(CountRefPtr && other)
		: ref{other.ref}
	{
		other.ref = nullptr;
	}

	/**
	 * Copy assignment
	 */
	inline CountRefPtr & operator=(const CountRefPtr & other)
	{
		// Release old reference
		if (ref) ref->release();

		// Acquire new reference
		if ((ref = other.ref)) ref->acquire();

		return *this;
	}

	/**
	 * Move assignment
	 */
	inline CountRefPtr & operator=(CountRefPtr && other)
	{
		ref = other.ref;
		other.ref = nullptr;

		return * this;
	}

	/**
	 * Destructor, remove reference from counter
	 */
	inline ~CountRefPtr()
	{
		if (ref) ref->release();
	}

	/**
	 * Return reference to object
	 */
	inline RefPtr<T> & operator*()
	{

	}

	/**
	 * Return pointer to object
	 */
	inline RefPtr<T> * operator->()
	{
		return ref;
	}
};