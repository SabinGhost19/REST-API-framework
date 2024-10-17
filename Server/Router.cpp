#include "Router.h"

void Router::addRoute(const std::string &method,
                      const std::string &path,
                      RouteCallback callback_function)
{
    routes[method + path] = callback_function;
}
void Router::route(Request &req, Response &res)
{
    auto key = req.GetPath() + req.GetMethod();

    if (this->routes.find(key) != this->routes.end())
    {
        this->routes[key](req, res);
    }
    else
    {
        res.SetStatusCode(404);
        std::string bdy = "Not Found";
        res.send(bdy);
    }
}