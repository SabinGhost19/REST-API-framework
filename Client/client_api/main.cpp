#include "W_FrREST.h"

#define PORT 443

int main()
{

    W_FrREST fetch = W_FrREST(PORT, "httpbin.org");

    std::string request = "GET /get HTTP/1.1\r\n"
                          "Host: catfact.ninja\r\n"
                          "Connection: close\r\n\r\n";
    json my_json = {
        {"nume", "sabin"},
        {"varsta", "21"}};

    std::string response = fetch.get("/get", json::object());
    // std::string response2 = fetch.post("/urs/usr1", my_json);

    std::cout << "Response: " << response;

    return 0;
}