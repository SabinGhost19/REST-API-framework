#include "Response.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "json.hpp"
#include <sys/types.h>
#include <netdb.h>
#include <sstream>
#include "Utils.h"

void Response::SetHeader(const std::string &name, const std::string &value)
{
    this->headers[name] = value;
}
void Response::SetStatusCode(int status_code = 0)
{
    this->statusCode = status_code;
}
std::string Response::Send(const std::string &body)
{
    std::ostringstream request;
    std::cout << "Status: " << statusCode << "\n";
    request << "HTTP/1.1 ";
    request << getStatusMessage(this->statusCode) << "\r\n";

    this->headers["Content-Length"] = body.size();
    for (const auto &header : headers)
    {
        request << header.first << ": " << header.second << "\n";
    }
    request << "\n";

    request << body;

    std::cout << "Response formed: " << request.str();

    // trimitem requestul
    send(this->client_fd, request.str().c_str(), request.str().length(), 0);
    std::cout << "HTTP request sent\n"
              << std::endl;
    std::cout << "Request SENT: " << request.str() << std::endl;

    char buffer[1024] = {0};

    // Citim raspunsul serverului
    int valread = read(this->client_fd, buffer, 1024);

    close(this->client_fd);

    return buffer;
}
std::string Response::Send(const char *body)
{
    std::ostringstream request;
    std::cout << "Status: " << statusCode << "\n";
    request << "HTTP/1.1 ";
    request << getStatusMessage(this->statusCode) << "\r\n";

    int length = 0;
    // Parcurgem șirul până întâlnim caracterul nul ('\0')
    while (body[length] != '\0')
    {
        length++;
    }
    std::string lenght_str = std::to_string(length);
    this->headers["Content-Length"] = lenght_str.c_str();

    for (const auto &header : headers)
    {
        request << header.first << ": " << header.second << "\n";
    }
    request << "\n";

    request << body;

    std::cout << "Response formed: " << request.str();

    // trimitem requestul
    send(this->client_fd, request.str().c_str(), request.str().size(), 0);
    std::cout << "HTTP request sent\n"
              << std::endl;
    std::cout << "Request SENT: " << request.str() << std::endl;

    char buffer[1024] = {0};

    // Citim raspunsul serverului
    int valread = read(this->client_fd, buffer, 1024);

    close(this->client_fd);

    return buffer;
}
Response::Response(int client_fd)
{
    this->statusCode = 200;
    this->client_fd = client_fd;
    // HTTP/1.1 200 OK
    // Connection: close
    // Content-Length: 0
    // Content-Type: application/json
    // Host: localhost
    // Server: MyHttpServer/1.0
    // Date: Wed, 21 Oct 2023 07:28:00 GMT

    headers["Connection"] = "close";
    headers["Content-Length"] = "0"; // Va fi setat corespunzător mai târziu
    headers["Content-Type"] = "application/json";
    headers["Host"] = "localhost";
    headers["Server"] = "MyHttpServer/1.0";
    // headers["Date"] = getCurrentDate();
}

std::string Response::Send(const json &body)
{

    std::ostringstream request;
    std::cout << "Status: " << statusCode << "\n";
    request << "HTTP/1.1 ";
    request << getStatusMessage(this->statusCode) << "\r\n";

    std::string body_str = jsonToString(body);
    std::string lenght = std::to_string(body_str.length());
    headers["Content-Length"] = lenght;

    for (const auto &header : headers)
    {
        request << header.first << ": " << header.second << "\n";
    }
    request << "\n";

    request << body_str;

    std::cout << "Response formed: " << request.str();

    // trimitem requestul
    send(this->client_fd, request.str().c_str(), request.str().length(), 0);
    std::cout << "HTTP request sent\n"
              << std::endl;
    std::cout << "Request SENT: " << request.str() << std::endl;

    if (this->headers["Connection"] == "close")
    {
        close(this->client_fd);

        return "--connection_closed--<empty>";
    }
    else
    {
        // pentru keep-alive sau upgrade
        char buffer[1024] = {0};

        // Citim raspunsul serverului
        int valread = read(this->client_fd, buffer, 1024);

        close(this->client_fd);

        return buffer;
    }
    return "";
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

void Response::Content_Type(ContentType type)
{
    std::string mssg = getContentTypeHeader(type);

    this->headers["Content-Type"] = mssg.c_str();
}

void Response::Connection_Type(ConnectionType type)
{
    std::string mssg = getConnectionHeader(type);

    headers["Connection"] = mssg.c_str();
}