#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "json.hpp"
#include <sys/types.h>
#include <netdb.h>

using json = nlohmann::json;

class W_FrREST
{

private:
    int _socket = 0;
    const char *ip_addr = "";
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    void BuildSocket(const addrinfo *res);
    void Serv_addr_Init(const addrinfo *res);
    void Connect_Sck_to_Srvr(const addrinfo *res);
    std::string Sent(const std::string &request);

public:
    W_FrREST(int PORT, const char *ip_addr);

    std::string get(const std::string &uri, const std::string &request);
    std::string post(const std::string &uri, const std::string &request);
    std::string put(const std::string &uri, const std::string &request);
    std::string patch(const std::string &uri, const std::string &request);
    std::string _delete(const std::string &uri, const std::string &request);

    std::string get(const std::string &uri, const json &request);
    std::string post(const std::string &uri, const json &request);
    std::string put(const std::string &uri, const json &request);
    std::string patch(const std::string &uri, const json &request);
    std::string _delete(const std::string &uri, const json &request);
};
