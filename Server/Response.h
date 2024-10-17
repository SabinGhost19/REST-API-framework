#include <iostream>
#include <map>
#include "json.hpp"

using nlohmann::json;

class Response
{

private:
    std::map<std::string, std::string> headers;
    int statusCode = 0;
    json stringToJson(const std::string &jsonString);
    std::string jsonToString(const json &jsonObj);

public:
    Response() : statusCode(200) {}
    void SetHeader(const std::string &name, const std::string &value);
    void SetStatusCode(int status_code);
    void send(const std::string &body);
    void send(const json &body);
    void send(const char *body);
};