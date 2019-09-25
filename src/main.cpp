#include <worker.h>
#include <worker_pool.h>
#include <iostream>
#include <unistd.h>
#include <future.h>

int main()
{
	srand(time(nullptr));
	unsigned int jobCount = 0;

	WorkerPool pool;
	pool.setNumWorkers(2);
	pool.launch();

	FutureBase<unsigned int> x;
	FutureBase<unsigned int> y;
	FutureBase<unsigned int> z;

	x.init();
	y.init();
	z.init();

	union
	{
		struct { unsigned int x, y, z; };
		unsigned int arr[3];
	} values[1024] = {}, sum;

	for (unsigned int i = 0; i < sizeof(values) / sizeof(*values); ++i)
		values[i] = {1, 2, 3};

	pool.submitJob([&x, &values]() mutable {

		// Compute sum over x values
		unsigned int sum = 0;
		for (unsigned int i = 0; i < sizeof(values) / sizeof(*values); ++i)
			sum += values[i].x;

		// Set future value
		x.set(sum);
	});

	pool.submitJob([&y, &values]() mutable {

		// Compute sum over x values
		unsigned int sum = 0;
		for (unsigned int i = 0; i < sizeof(values) / sizeof(*values); ++i)
			sum += values[i].y;

		// Set future value
		y.set(sum);
	});

	pool.submitJob([&z, &values]() mutable {

		// Compute sum over x values
		unsigned int sum = 0;
		for (unsigned int i = 0; i < sizeof(values) / sizeof(*values); ++i)
			sum += values[i].z;

		// Set future value
		z.set(sum);
	});

	x.get(sum.x);
	y.get(sum.y);
	z.get(sum.z);

	printf("(%d, %d, %d)\n", sum.x, sum.y, sum.z);

	return 0;
}