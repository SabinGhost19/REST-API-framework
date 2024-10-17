#include "Response.h"

void Response::SetHeader(const std::string &name, const std::string &value)
{
    this->headers[name] = value;
}
void Response::SetStatusCode(int status_code = 0)
{
    this->statusCode = status_code;
}
void Response::send(const std::string &body)
{
    std::ostringstream request;

    for (const auto &header : headers)
    {
        request << header.first << ": " << header.second << "\n";
    }
    request << "\n";

    if (!body.empty())
    {
        std::string body_str = jsonToString(body);
        request << body_str;
    }
    std::cout << "Response formed: " << request.str();
}
void Response::send(const char *body)
{
    std::ostringstream request;
    std::cout << "Status: " << statusCode << "\n";
    for (const auto &header : headers)
    {
        request << header.first << ": " << header.second << "\n";
    }
    request << "\n";

    request << body;
    std::cout << "Response formed: " << request.str();
}
void Response::send(const json &body)
{

    std::ostringstream request;
    std::cout << "Status: " << statusCode << "\n";
    for (const auto &header : headers)
    {
        request << header.first << ": " << header.second << "\n";
    }
    request << "\n";

    if (!body.empty())
    {
        std::string body_str = jsonToString(body);
        request << body_str;
    }
    std::cout << "Response formed: " << request.str();
}

json Response::stringToJson(const std::string &jsonString)
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
std::string Response::jsonToString(const json &jsonObj)
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