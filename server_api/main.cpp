#include "server.cpp"

int main()
{
    RestServer server(8080);
    server.addRoute("GET", "/hello", hello_handler);
    server.run();
    return 0;
}