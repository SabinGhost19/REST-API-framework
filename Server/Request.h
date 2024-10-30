#ifndef REQUEST
#define REQUEST

#include <string.h>
#include <iostream>
#include <map>
#include <sstream>

class Request
{

private:
    std::string path = "";
    std::string method = "";
    std::string body = "";
    std::map<std::string, std::string> queryParams;
    std::map<std::string, std::string> headers;

public:
    std::string GetQueryParam(const std::string &key);
    std::string GetHeader(const std::string &key);
    void ParseQueryParams();

    Request(const std::string &path_, const std::string &method_, const std::string &body_, const std::map<std::string, std::string> &headers_) : path(path_), method(method_), body(body_), headers(headers_) {}
    Request() {}
    std::map<std::string, std::string> GetHeaders() { return headers; }
    std::string GetPath() { return this->path; }
    std::string GetMethod() { return this->method; }
    std::string GetBody() { return this->body; }
};

#endif // REQUEST
