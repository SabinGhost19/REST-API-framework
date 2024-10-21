#include "HTTPServer.h"

RestServer::RestServer(int port) : port(port)
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Acceptăm conexiuni de pe toate IP-urile
    address.sin_port = htons(port);

    // Legarea socket-ului la adresa și portul specificate
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Serverul va asculta pentru conexiuni
    if (listen(server_fd, 5) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
}

RestServer::~RestServer()
{
    close(server_fd);
}

void RestServer::addRouter(Router *_router)
{
    this->router = _router;
}

void RestServer::run()
{
    std::cout << "Server is listening on port " << port << std::endl;
    while (true)
    {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

        if (client_fd < 0)
        {
            perror("Accept failed");
            continue;
        }
        std::cout << "Client connected!" << std::endl;
        handle_client(client_fd);
        close(client_fd);
    }
}

// Tratarea unei cereri client
void RestServer::handle_client(int client_fd)
{
    char buffer[1024] = {0};
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
    if (bytes_read > 0)
    {

        std::string request(buffer);
        buffer[bytes_read] = '\0'; // Adaugă terminator de string
        std::cout << "Request received:\n"
                  << buffer << std::endl;

        std::string route = parse_request(request);

        std::string method = request.substr(0, request.find(" "));
        std::string key = route + method;

        Response res(client_fd);
        Request req(route, method, "");

        this->router->route(req, res);
        // if (routes.find(key) != routes.end())
        // {
        //     // Găsim handler-ul rutei
        //     std::string response = routes[key](request);
        //     write(client_fd, response.c_str(), response.length());
        // }
        // else
        // {
        //     // Răspuns 404
        //     std::string response = "HTTP/1.1 404 Not Found\r\n\r\n";
        //     write(client_fd, response.c_str(), response.length());
        // }
    }
}

std::string RestServer::parse_request(const std::string &request)
{
    std::istringstream iss(request);
    std::string method, route, version;
    iss >> method >> route >> version; // Extragem metoda, ruta și versiunea
    return route;                      // Returnăm ruta
}

// // Un exemplu de handler pentru ruta /hello
// std::string hello_handler(const std::string &request)
// {
//     return "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
// }