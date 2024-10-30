#ifndef RESPONSE
#define RESPONSE

#include <iostream>
#include <map>
#include "json.hpp"
#include <fstream>

using nlohmann::json;
enum class ContentType;
enum class ConnectionType;
class Response
{

private:
    std::string body;
    std::map<std::string, std::string> headers;
    int statusCode = 0;
    json stringToJson(const std::string &jsonString);
    std::string jsonToString(const json &jsonObj);
    int client_fd = 0;
    void Send();
    void SetBody(const std::string &body_);

public:
    Response(int);
    Response();
    void SetHeader(const std::string &name, const std::string &value);
    void SetStatusCode(int status_code);
    std::string Send(const std::string &body);
    std::string Send(const json &body);
    std::string Send(const char *body);
    void SendFile(const std::string &file_path);
    void Content_Type(ContentType type);
    void Connection_Type(ConnectionType type);
};

#endif // RESPONSE