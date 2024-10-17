#ifndef ROUTER_H
#define ROUTER_H
#include <map>
#include <functional>
#include "Request.h"
#include "Response.h"

class Router
{

private:
    using RouteCallback = std::function<void(Request &, Response &)>;
    std::map<std::string, RouteCallback> routes;

public:
    void addRoute(const std::string &method,
                  const std::string &path,
                  RouteCallback callback_function);
    void route(Request &req, Response &res);
};

#endif