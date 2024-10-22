#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <arpa/inet.h>
#include <unistd.h>
#include "Router.h"
#include "json.hpp"
#include "ThreadPool.h"

typedef std::string (*RouteHandler)(const std::string &);

class RestServer
{
public:
    RestServer(int port, int number_of_threads);
    ~RestServer();
    void addRouter(Router *_router);
    void run();

private:
    Router *router = NULL;
    ThreadPool *threadPool = NULL;
    int port;
    int server_fd;
    std::unordered_map<std::string, RouteHandler> routes;

    void handle_client(int client_fd);
    std::string parse_request(const std::string &request);
};