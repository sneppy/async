#include <critical_section.h>
#include <event.h>
#include <functional>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <worker.h>
#include <worker_pool.h>

#define MAKE_IPV4(x, y, z, w) ((x << 24u) | (y << 16u) | (z << 8u) | w)

class ServerManager
{
protected:

public:
};

ServerManager * gServerManager = nullptr;

class Server
{
public:
	//////////////////////////////////////////////////
	// Server interface
	//////////////////////////////////////////////////
	
	virtual void init() {};
	virtual void start() = 0;
	virtual void shutdown() {};
};

class Route
{
public:
	Route & setName(const char * name) {};
};

class Router
{
public:
	//////////////////////////////////////////////////
	// Router interface
	//////////////////////////////////////////////////
	
	Route & GET(const char * route, std::function<void()> && handle) {};
	Route & POST(const char * route, std::function<void()> && handle) {};
	Route & PUT(const char * route, std::function<void()> && handle) {};
	Route & DELETE(const char * route, std::function<void()> && handle) {};
};

class HttpServer : public Server
{
protected:
	/// Server router
	Router * router;

	/// Socket descriptor
	int serverSocket;

	/// Worker pool
	WorkerPool * pool;

protected:
	/**
	 * 
	 */
	virtual void registerRoutes()
	{
		//
	}

public:
	//////////////////////////////////////////////////
	// Server interface
	//////////////////////////////////////////////////

	virtual void init() override
	{
		unsigned int val = 1;

		// Create a new TCP socket
		serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
		setsockopt(serverSocket, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));

		// Create router
		router = new Router;

		// Create worker pool
		pool = new WorkerPool;
		pool->setNumWorkers(7);
	}
	
	virtual void start() override
	{
		// Bind socket
		{
			sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_port = htons(5000);
			addr.sin_addr.s_addr = INADDR_ANY;
			::bind(serverSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
		}

		// Register routes
		registerRoutes();

		// Start workers
		pool->launch();

		// Start listening on port
		::listen(serverSocket, 8);

		for (;;)
		{
			sockaddr_storage addr;
			socklen_t addrLen = 0;
			int clientSocket = ::accept(serverSocket, reinterpret_cast<sockaddr*>(&addr), &addrLen);


			if (clientSocket != -1)
			{
				pool->submitJob([clientSocket]() {

					char buff[1024];

					size_t size = 0;
					do
					{
						::recv(clientSocket, buff, 1024, 0);
						//std::cout << buff;
					} while (size == 1024);

					// Send back Http response
					constexpr char res[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!doctype html><html><head><title>D&D</title></head><body><h1>Hello world!</h1></body></html>";
					::strcpy(buff, res);
					::send(clientSocket, buff, sizeof(res) - 1, 0);

					::close(clientSocket);
				});
			}
		}
	}
	
	virtual void shutdown() override
	{
		// Delete resources
		delete router;
		delete pool;
	}
};

class DDServer : public HttpServer
{
protected:
	//////////////////////////////////////////////////
	// HttpServer interface
	//////////////////////////////////////////////////
	
	virtual void registerRoutes() override
	{
		router->GET("/adventure/{id}/bus", []() {

			std::cout << "Hello world!" << std::endl;
		});
	}
};

int main()
{
	gServerManager = new ServerManager;

	Server * server = new DDServer;
	server->init();
	server->start();
	server->shutdown();

	return 0;
}