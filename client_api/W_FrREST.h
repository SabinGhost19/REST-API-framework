#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class W_FrREST
{

private:
    int _socket = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    void BuildSocket();
    void Serv_addr_Init(int PORT);
    void Connect_Sck_to_Srvr(const char *ip_addr);

public:
    W_FrREST(int PORT, const char *ip_addr);
    std::string Sent(const std::string &request);
};
