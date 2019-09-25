#include <event.h>

bool Event::wait()
{
	ScopeLock _{&mutex};
	bool status = false;

	do
	{
		// Add to queue
		++numWaiting;

		if (state == State::TRIGGER_ONE)
		{
			// Wake up only this thread
			state = State::TRIGGER_NONE;
			status = true;
		}
		else if (state == State::TRIGGER_ALL)
		{
			// Wake up all threads
			status = true;
		}
		else
		{
			// Wait for signal
			pthread_cond_wait(&cond, &mutex);
		}

		// Temporarily remove me from queue
		--numWaiting;
	} while (!status);

	return status;
}
