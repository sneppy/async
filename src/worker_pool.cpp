#include <worker_pool.h>
#include <worker.h>

void WorkerPool::setNumWorkers(unsigned int inNumThreads)
{
	// Destroy workers
	for (auto worker : workers)
		worker->~Worker();
	
	// Reset vector
	workers.clear();

	// Create new workers
	workers.resize(inNumThreads, nullptr);
	for (unsigned int i = 0; i < inNumThreads; ++i)
		workers[i] = new Worker{i, this};
}

void WorkerPool::launch()
{
	// Start workers
	for (auto worker : workers)
		worker->start();
}

bool WorkerPool::submitJob(std::function<void()> && inJob)
{
	ScopeLock _{&mutex};
	if (jobs.pushBack(std::move(inJob)))
	{
		// Fire event
		event->fire<Event::TRIGGER_ONE>();
		return true;
	}
	else return false;		
}