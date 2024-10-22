#include <iostream>
#include "HTTPServer.h"
#include "Utils.h"

#define PORT 8081
// Funcția care transformă JSON în string
std::string jsonToString(const json &jsonObj)
{
    try
    {
        // Serializarea obiectului JSON într-un string
        return jsonObj.dump();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Eroare la serializarea JSON-ului: " << e.what() << std::endl;
        return "";
    }
}
std::string createHttpRequest(const std::string &method, const std::string &uri, const std::string &http_version,
                              const std::map<std::string, std::string> &headers, const json &jsonBody = {})
{
    std::ostringstream request;

    // 1. Linia de start: METHOD URI HTTP_VERSION
    request << method << " " << uri << " " << http_version << "\r\n";

    // 2. Headerele
    for (const auto &header : headers)
    {
        request << header.first << ": " << header.second << "\r\n";
    }

    // 3. Linia goală care indică sfârșitul headerelor
    request << "\r\n";

    // 4. Corpul (dacă există)
    if (!jsonBody.empty())
    {
        std::string body = jsonToString(jsonBody);
        request << body;
    }

    return request.str();
}

void functieptGET(Request &req, Response &res)
{
    std::string body = req.GetBody();
    std::cout << body << std::endl;

    std::string r = req.GetMethod();
    std::cout << "Developer Function: \nMethod : " << r << "\n Body : " << body << std::endl;

    json jsonBody = {
        {"username", "john"},
        {"password", "1234"}};
    res.SetStatusCode(202);
    res.Connection_Type(ConnectionType::Close);
    res.Content_Type(ContentType::ApplicationJson);
    res.Send(jsonBody);

    // res.SetStatusCode(202);
    // res.Connection_Type(ConnectionType::Close);
    // res.Content_Type(ContentType::TextPlain);
    // res.Send("mama are mere const char***");

    // std::string string_my = "server tring....";
    // res.Send(string_my);
}

int main()
{
    // Apache Benchmark -testing command
    // ab -n 100 -c 10 http://localhost:8081/home
    // ab -n 100 -c 2 -s 60 http://localhost:8081/home
    //...not working...????????

    // httperf:
    // httperf --server localhost --port 8081 --uri /home --num-conns 1500 --rate 50

    Router *router = new Router();
    router->addRoute("GET", "/home", functieptGET);

    RestServer server(PORT, 5);
    server.addRouter(router);

    server.run();

    return 0;
}