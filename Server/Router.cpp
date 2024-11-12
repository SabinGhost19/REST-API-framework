#include "Router.h"

void Router::addRoute(const std::string &method,
                      const std::string &path,
                      RouteCallback callback_function)
{
     std::string key = method + path;
    routes[key] = callback_function;
    std::cout << "Ruta adăugată: " << key << std::endl;
}
void Router::route(Request &req, Response &res)
{
    std::string key = req.GetMethod() + req.GetPath();
    std::cout << "Routing key: " << key << std::endl;

    for (const auto &route_entry : routes)
    {
        const std::string &route_key = route_entry.first;
        std::regex route_regex(route_key);
        std::smatch match;

        if (std::regex_match(key, match, route_regex))
        {
            std::regex param_regex(R"(:\w+)");
            std::sregex_iterator iter(route_key.begin(), route_key.end(), param_regex);
            std::sregex_iterator end;

            size_t index = 1;
            while (iter != end)
            {
                std::string param_name = (*iter).str().substr(1);
                req.SetRouteParam(param_name, match[index++]);
                ++iter;
            }

            route_entry.second(req, res);
            return;
        }
    }

    res.SetStatusCode(404);
    std::string body = "Not Found";
    res.Send(body);
}