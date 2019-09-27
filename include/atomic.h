#pragma once

/**
 * Atomic type
 * 
 * @param [in] IntT integral type
 */
template<typename IntT>
class Atomic
{
protected:
	/// Volatile storage
	volatile IntT data;

public:
	/**
	 * Set initial value
	 */
	Atomic(IntT inData = IntT{})
		: data{inData}
	{
		//
	}

	/**
	 * Returns the underlying value
	 */
	inline IntT get() const
	{
		IntT out; __atomic_load((volatile IntT*)&data, &out, __ATOMIC_SEQ_CST);
		return out;
	}

	/**
	 * Atomically set value
	 */
	inline void set(IntT inData)
	{
		__atomic_store(&data, &inData, __ATOMIC_SEQ_CST);
	}

	/**
	 * 
	 */
	inline IntT operator++()
	{
		return __sync_fetch_and_add(&data, 1) + 1;
	}

	/**
	 * 
	 */
	inline IntT operator++(int)
	{
		return __sync_fetch_and_add(&data, 1);
	}

	/**
	 * 
	 */
	inline IntT operator--()
	{
		return __sync_fetch_and_sub(&data, 1) + 1;
	}

	/**
	 * 
	 */
	inline IntT operator--(int)
	{
		return __sync_fetch_and_sub(&data, 1);
	}

	/**
	 * Returns a copy of the underlying value
	 */
	inline operator IntT() const
	{
		return get();
	}
};