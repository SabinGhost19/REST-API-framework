#include "W_FrREST.h"

W_FrREST::W_FrREST(int PORT, const char *ip_addr)
{

    this->BuildSocket();
    this->Serv_addr_Init(PORT);
    this->Connect_Sck_to_Srvr(ip_addr);
}
void W_FrREST::BuildSocket()
{

    if ((this->_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Socket creation error\n";
        // throw(Error);
    }
}

void W_FrREST::Serv_addr_Init(int PORT)
{
    // initializarea adresei serverului
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
}

void W_FrREST::Connect_Sck_to_Srvr(const char *ip_addr)
{

    if (inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address / Address not supported\n";
        // return -1;
    }

    // Conectam socketul la serverul de pe localhost
    if (connect(this->_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Connection failed\n";
        // return -1;
    }
}
std::string W_FrREST::Sent(const std::string &request)
{
    // trimitem requestul
    send(this->_socket, request.c_str(), request.length(), 0);
    std::cout << "HTTP request sent\n";

    buffer[1024] = {0};

    // Citim raspunsul serverului
    int valread = read(this->_socket, buffer, 1024);

    close(this->_socket);

    return buffer;
}