#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

void handleClient(int client_socket)
{
    char buffer[1024] = {0};
    // citire request
    read(client_socket, buffer, 1024);
    std::cout << "Request received:\n"
              << buffer << std::endl;

    std::string response = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/plain\r\n"
                           "Content-Length: 21\r\n"
                           "Connection: close\r\n\r\n"
                           "GET request received.";

    // send response
    send(client_socket, response.c_str(), response.size(), 0);

    close(client_socket);
}

int main()
{
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "Socket creation failed\n";
        exit(EXIT_FAILURE);
    }

    // Configurăm opțiunea pentru reutilizarea adresei și portului
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cerr << "Setsockopt failed\n";
        exit(EXIT_FAILURE);
    }

    // Configurăm adresa serverului
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // bind socket la port/adresa construita
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed\n";
        exit(EXIT_FAILURE);
    }

    // ascultam
    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "Listen failed\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Server running on port " << PORT << std::endl;

    // Acceptăm conexiuni și gestionăm cererile clienților
    while (true)
    {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            std::cerr << "Accept failed\n";
            exit(EXIT_FAILURE);
        }

        // gestionare cerere/request
        handleClient(client_socket);
    }

    return 0;
}
