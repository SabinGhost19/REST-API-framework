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
        std::cout << "Original route_key: " << route_key << std::endl;

        // Înlocuirea parametrilor `:param` cu un regex care să facă match pe valori
        std::string route_key_regex_str = std::regex_replace(route_key, std::regex(R"(:\w+)"), "([^/]+)");
        std::regex route_regex("^" + route_key_regex_str + "$");

        std::cout << "Regex-ul route_key: " << route_key_regex_str << std::endl;

        std::smatch match;
        if (std::regex_match(key, match, route_regex))
        {
            std::cout << "Route matched: " << route_entry.first << std::endl;

            // Împărțim ruta cerută și ruta definită în router pentru a obține părțile separate de `/`
            std::vector<std::string> request_parts = splitPath(req.GetPath(), '/');
            std::vector<std::string> route_parts = splitPath(route_key.substr(route_key.find("/") + 1), '/');

            // Parcurgem fiecare parte a rutei și extragem parametrii dinamici (prefixați cu `:`)
            for (size_t i = 0; i < route_parts.size(); ++i)
            {
                if (route_parts[i].size() > 0 && route_parts[i][0] == ':')
                {
                    std::string param_name = route_parts[i].substr(1); // eliminăm `:`
                    if (i < request_parts.size())
                    {
                        std::cout<<"!!!!!!"<<request_parts[i]<<param_name<<std::endl;
                        req.SetRouteParam(param_name, request_parts[i]);
                        std::cout << "Param extracted: " << param_name << " = " << request_parts[i] << std::endl;
                    }
                    else
                    {
                        std::cerr << "Error: Invalid index for request_parts." << std::endl;
                        res.SetStatusCode(500);
                        res.Send("Internal Server Error");
                        return;
                    }
                }
            }

            // Apelăm funcția de handler corespunzătoare
            route_entry.second(req, res);
            return;
        }
    }

    // Dacă nu găsim niciun match, trimitem 404
    res.SetStatusCode(404);
    std::string body = "Not Found";
    res.Send(body);
}

std::vector<std::string> Router::splitPath(const std::string &path, char delimiter)
{
    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string item;

    while (std::getline(ss, item, delimiter))
    {
        // Ignorăm părțile goale care pot apărea din cauza unui `/` la începutul rutei
        if (!item.empty())
        {
            parts.push_back(item);
        }
    }

    return parts;
}