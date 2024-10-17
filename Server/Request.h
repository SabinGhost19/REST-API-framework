#include <string.h>
#include <iostream>

class Request
{

private:
    std::string path = "";
    std::string method = "";
    std::string body = "";

public:
    Request(const std::string &path_, const std::string &method_, const std::string &body_) : path(path_), method(method_), body(body_) {}

    std::string GetPath() { return this->path; }
    std::string GetMethod() { return this->method; }
    std::string GetBody() { return this->body; }
};
