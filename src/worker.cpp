#include <worker.h>
#include <worker_pool.h>

void Worker::start()
{
	// Create thread resource
	if (thread) delete thread;
	thread = new pthread_t;

	// Run thread
	pthread_create(thread, nullptr, &launchWorkerThread, this);
}

void * Worker::launchWorkerThread(void * worker)
{
	Worker * self = reinterpret_cast<Worker*>(worker);
	
	if (self)
	{
		// Worker life cycle
		self->init();
		self->run();
		self->shutdown();
	}

	return nullptr;
}

void Worker::run()
{
	for (;;)
	{
		std::function<void()> job;
		while (!owner->getPendingJob(job)) owner->waitForJob(this);

		// We have job, let's do this!
		job();
	}
}
