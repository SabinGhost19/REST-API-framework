#include <iostream>
#include "Router.h"

class HTTP_Server
{

private:
public:
    HTTP_Server();
    void handleClient(int client_socket);
    void listen();
};