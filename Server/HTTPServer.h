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
#include "MiddleWare.h"

typedef std::string (*RouteHandler)(const std::string &);

class RestServer
{
public:
    RestServer(int port, int number_of_threads);
    ~RestServer();
    void addRouter(Router *_router);
    void run();
    void use(MiddleWare::MidW_Handler middleWare_function);
    int nr_of_requests = 0;

private:
    MiddleWare *middleWare = NULL;
    Router *router = NULL;
    ThreadPool *threadPool = NULL;
    int port;
    int server_fd;
    std::unordered_map<std::string, RouteHandler> routes;
    std::pair<Request,Response> receiveTheRequest(int client_fd);
    void handle_client(Request &req,Response &res);
};