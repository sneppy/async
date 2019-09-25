#include <worker.h>
#include <worker_pool.h>
#include <iostream>
#include <unistd.h>

int main()
{
	srand(time(nullptr));
	unsigned int jobCount = 0;

	WorkerPool pool;
	pool.setNumWorkers(7);
	pool.launch();

	for (;; /* usleep(500000ull) */)
	{
		getc(stdin);
		std::cout << "submit job #" << jobCount << std::endl;
		pool.submitJob([jobId = jobCount]() {
			
			std::cout << "working job #" << jobId << std::endl;
			usleep(1000000ull + rand() % 1000000);
			std::cout << "done job#" << jobId << std::endl;
		}) && ++jobCount;
	}

	return 0;
}