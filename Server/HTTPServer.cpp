#include "HTTPServer.h"
#include <filesystem>
#include <fstream>

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
RestServer::RestServer(int port, int number_of_threads, const std::string &rootPath) : port(port)
{  
    this->threadPool = new ThreadPool(number_of_threads);
    this->middleWare = new MiddleWare();

    this->setupProjectStructure(rootPath);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(port);

   
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

   
    if (listen(server_fd, 5) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

}
void RestServer::setupProjectStructure(const std::string &rootPath) {
    using namespace std::filesystem;

    create_directories(rootPath + "/migrations");
    create_directories(rootPath + "/models");
    create_directories(rootPath + "/repository");
    create_directories(rootPath + "/config");
    create_directories(rootPath + "/tests");
    create_directories(rootPath + "/logs");

    std::ofstream(rootPath + "/migrations/001_create_users_table.sql")
        << "-- SQL script to create users table\nCREATE TABLE users (id SERIAL PRIMARY KEY, name TEXT, email TEXT, password TEXT);";
    std::ofstream(rootPath + "/models/User.h")
        << "#ifndef USER_H\n#define USER_H\n#include <string>\nclass User {\npublic:\nint id;\nstd::string name, email, password;\n};\n#endif";
    std::ofstream(rootPath + "/config/database.json")
        << "{\n\"host\": \"localhost\",\n\"port\": 5432,\n\"dbname\": \"app_db\",\n\"user\": \"user\",\n\"password\": \"password\"\n}";
    std::ofstream(rootPath + "/logs/README.md") << "This folder contains application and database logs.";
    std::ofstream(rootPath + "/tests/README.md") << "This folder contains unit and integration tests.";
    std::ofstream repoInterfaceFile(rootPath + "/repository/IRepository.h");
    repoInterfaceFile << "#ifndef IREPOSITORY_H\n"
                      << "#define IREPOSITORY_H\n\n"
                      << "#include <string>\n"
                      << "#include <vector>\n"
                      << "#include <map>\n\n"
                      << "template <typename T>\n"
                      << "class IRepository {\n"
                      << "public:\n"
                      << "    virtual bool add(const T& entity) = 0;\n"
                      << "    virtual bool remove(int id) = 0;\n"
                      << "    virtual bool update(const T& entity) = 0;\n"
                      << "    virtual T getById(int id) = 0;\n"
                      << "    virtual std::vector<T> getAll() = 0;\n"
                      << "    virtual bool exists(int id) = 0;\n"
                      << "    virtual bool exists(const std::string& field, const std::string& value) = 0;\n"
                      << "    virtual std::vector<T> findByField(const std::string& field, const std::string& value) = 0;\n"
                      << "    virtual int count() = 0;\n"
                      << "    virtual bool removeAll() = 0;\n"
                      << "    virtual std::vector<T> getAllSorted(const std::string& field, bool ascending = true) = 0;\n"
                      << "    // Others...\n"
                      << "    // virtual std::vector<T> findByCriteria(const std::map<std::string, std::string>& criteria) = 0;\n"
                      << "    // virtual std::vector<T> findByRange(const std::string& field, double min, double max) = 0;\n"
                      << "    // virtual std::vector<T> findByDateRange(const std::string& field, const std::string& startDate, const std::string& endDate) = 0;\n\n"
                      << "    virtual ~IRepository() = default;\n"
                      << "};\n\n"
                      << "#endif\n";
    repoInterfaceFile.close();


    std::cout << "Project structure created successfully in: " << rootPath << std::endl;
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
    this->middleWare->use_this_specific_middleWare(endpoint, [this](Request& req, Response& res, std::function<void()> next) {
        this->middleWare->authentication_middleware(req, res, next);
    });
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
                                               if (req.GetMethod() == "OPTIONS")
                                                {
                                                   res.SetHeader("Access-Control-Allow-Origin", "*"); // Sau specifică originea exactă
                                                    res.SetHeader("Access-Control-Allow-Methods", "GET, POST, PUT, PATCH, DELETE, OPTIONS");
                                                    res.SetHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
                                                    
                                                    // Răspunde cu status 200 OK
                                                    res.SetStatusCode(200);
                                                    res.Send(""); // Răspuns gol
                                                    return; // Oprește procesarea cererii
                                                }

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