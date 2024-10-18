#include <iostream>
#include "HTTP_Server.h"
#include "./temp/HTTPServer.h"

#define PORT 8080
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
    std::cout << "method:" << r << std::endl;

    res.SetStatusCode(204);
    res.send("BUna de la server");
}

int main()
{
    Router *router = new Router();
    router->addRoute("GET", "/home", functieptGET);

    RestServer server(PORT);
    server.addRouter(router);

    server.run();
    // std::map<std::string, std::string> headers = {
    //     {"Host", "example.com"},
    //     {"Content-Type", "application/json"},
    //     {"Content-Length", "27"}};

    // // Creăm un obiect JSON pentru corpul cererii
    // json jsonBody = {
    //     {"username", "john"},
    //     {"password", "1234"}};

    // // Crearea request-ului HTTP cu JSON
    // std::string httpRequest = createHttpRequest("GET", "/home", "HTTP/1.1", headers, jsonBody);

    // std::string method = httpRequest.substr(0, httpRequest.find(' '));
    // std::string path = httpRequest.substr(httpRequest.find(' ') + 1);

    // path = path.substr(0, path.find(' '));

    // Request request(method, path, ""); // for simplicity, no body parsing
    // Response response;

    // router.route(request, response);

    return 0;
}