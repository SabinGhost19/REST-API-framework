#include "HTTPServer.h"

RestServer::RestServer(int port, int number_of_threads) : port(port)
{
    this->threadPool = new ThreadPool(number_of_threads);
    this->middleWare = new MiddleWare();

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
void RestServer::setAuthEmails(std::vector<std::string>vec){
    this->middleWare->setAuthEmails(std::move(vec));
}

void RestServer::use(MiddleWare::MidW_Handler middleWare_function)
{
    // add the middleWare in the middleWares functional pool
    this->middleWare->use_this_middleWare(middleWare_function);
}
void RestServer::use(std::string endpoint,MiddleWare::MidW_Handler middleWare_function)
{
    // add the middleWare in the middleWares functional pool
    this->middleWare->use_this_specific_middleWare(endpoint,middleWare_function);
}
void RestServer::useSimpleAuthMiddleware(std::string endpoint){
    this->middleWare->use_this_specific_middleWare(endpoint,this->middleWare->authentication_middleware);
}
void RestServer::run()
{
    std::cout << "Server is listening on port " << this->port << std::endl;
    while (true)
    {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        this->nr_of_requests++;
        printf("\n\n\n\n..................NR OF REQ: %d.............\n\n", this->nr_of_requests);
        if (client_fd < 0)
        {
            perror("Accept failed");
            continue;
        }

        try
        {
            // enqueue de functie-> lambda si definirea sa
            this->threadPool->enqueue([this, client_fd]
                                      {
                                          try
                                          {
                                            //receiving the actual request with all its datas
                                            //and popualte the respective strucures
                                            //to be used in the middlewares
                                            //and of course in the handling functions
                                              std::pair<Request,Response>populated_pair=this->receiveTheRequest(client_fd);
                                              Request req=populated_pair.first;
                                              Response res=populated_pair.second;
                                              std::cout<<".................."<<req.GetMethod()<<"CALEEEEEAAAAAAAAAAAAAAAAAAAAAAA\n";
                                              this->middleWare->execute_middleWares(req, res, [this, &req, &res, client_fd]()
                                                                                    {
                                                                                        handle_client(req,res); // gestionam clientul/requestul
                                                                                        //call ce va fi passed drept functie next() in the middlewares
                                                                                                                  
                                                                                    });
                                          }
                                          catch (const std::exception &e)
                                          {
                                              std::cerr << "Error handling client: " << e.what() << std::endl;
                                          }
                                          catch (...)
                                          {
                                              std::cerr << "Unknown error occurred while handling client." << std::endl;
                                          }
                                          close(client_fd); // inchidem conexiunea cu clientul dupa ce am terminat
                                      });
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to enqueue client handling task: " << e.what() << std::endl;
            close(client_fd); // daca nu putem adauga task-ul in pool
        }
        catch (...)
        {
            std::cerr << "Unknown error occurred while enqueuing client handling task." << std::endl;
            close(client_fd); // daca nu putem adauga task-ul in pool
        }
    }
}
std::string parse_request(const std::string &request)
{
    size_t pathStart = request.find(" ") + 1;
    size_t pathEnd = request.find(" ", pathStart);
    return request.substr(pathStart, pathEnd - pathStart);
}

std::map<std::string, std::string> parse_headers(const std::string &request)
{
    std::map<std::string, std::string> headers;
    size_t headerStart = request.find("\r\n") + 2;
    size_t bodyStart = request.find("\r\n\r\n");
    if (bodyStart == std::string::npos)
    {
        return headers;
    }

    std::string headersString = request.substr(headerStart, bodyStart - headerStart);
    std::stringstream ss(headersString);
    std::string line;
    while (std::getline(ss, line))
    {
        if (line.find(": ") != std::string::npos)
        {
            size_t delimiterPos = line.find(": ");
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 2);
            if (!key.empty() && !value.empty())
            {
                headers[key] = value;
            }
        }
    }
    return headers;
}

std::string parse_body(const std::string &request)
{
    size_t bodyStart = request.find("\r\n\r\n");
    if (bodyStart != std::string::npos)
    {
        return request.substr(bodyStart + 4);
    }
    return "";
}
std::pair<Request,Response> RestServer::receiveTheRequest(int client_fd){

 char buffer[1024] = {0};
 ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
    if (bytes_read > 0)
    {
        buffer[bytes_read] = '\0';
        std::string request(buffer);
        std::cout << "Request received:\n"
                  << buffer << std::endl;

        // Parse method, route, headers, and body
        std::string route = parse_request(request);
        std::string method = request.substr(0, request.find(" "));
        auto headers = parse_headers(request);
        std::string body = parse_body(request);

        // Create Request object
        Request req(route, method, body, headers);
        std::cout << std::endl
                  << req.GetPath() << std::endl;

        req.ParseQueryParams();
        std::cout << std::endl
                  << req.GetPath() << std::endl;
        Response res(client_fd);
         return std::make_pair(req, res);
    }
    return std::make_pair(Request(), Response(client_fd));
}
// Tratarea unui request,call the method
void RestServer::handle_client(Request &req,Response &res)
{
        // Route the request
        this->router->route(req, res);
}