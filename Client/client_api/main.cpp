#include "W_FrREST.h"

#define PORT 8080

int main()
{

    W_FrREST fetch = W_FrREST(PORT, "127.0.0.1");

    std::string request = "GET /get HTTP/1.1\r\n"
                          "Host: localhost\r\n"
                          "Connection: close\r\n\r\n";

    std::string response = fetch.Sent(request);
    std::cout << "Response: " << response;

    return 0;
}