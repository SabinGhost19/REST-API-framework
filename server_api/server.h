#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <arpa/inet.h>
#include <unistd.h>

typedef std::string(*RouteHandler)(const std::string&);

class RestServer
{
    public:
        RestServer(int port);
        ~RestServer();
        void addRoute(const std::string& method, const std::string& route, RouteHandler handler);
        void run();
    private:
        int port;
        int server_fd;
        std::unordered_map<std::string, RouteHandler> routes;

        void handle_client(int client_fd);
        std::string parse_request(const std::string& request);
};