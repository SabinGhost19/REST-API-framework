#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include "json.hpp" // Include librăria JSON

// Alias pentru a simplifica referirea la nlohmann::json
using json = nlohmann::json;

// Funcția care transformă string în JSON
json stringToJson(const std::string &jsonString)
{
    try
    {
        // Parsează string-ul în JSON
        json jsonObj = json::parse(jsonString);
        return jsonObj;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Eroare la parsarea JSON-ului: " << e.what() << std::endl;
        return nullptr;
    }
}

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

// Funcția care construiește un request HTTP
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

int main()
{
    // Exemplu de request POST cu corp JSON
    std::map<std::string, std::string> headers = {
        {"Host", "example.com"},
        {"Content-Type", "application/json"},
        {"Content-Length", "27"}};

    // Creăm un obiect JSON pentru corpul cererii
    json jsonBody = {
        {"username", "john"},
        {"password", "1234"}};

    // Crearea request-ului HTTP cu JSON
    std::string httpRequest = createHttpRequest("POST", "/login", "HTTP/1.1", headers, jsonBody);

    // Afișarea request-ului creat
    std::cout << httpRequest << std::endl;

    return 0;
}
