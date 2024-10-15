#include "W_FrREST.h"

W_FrREST::W_FrREST(int PORT, const char *ip_addr)
{

    this->BuildSocket();
    this->Serv_addr_Init(PORT);
    this->Connect_Sck_to_Srvr(ip_addr);
}
void W_FrREST::BuildSocket()
{

    if ((this->_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Socket creation error\n";
        // throw(Error);
    }
}

void W_FrREST::Serv_addr_Init(int PORT)
{
    // initializarea adresei serverului
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
}

void W_FrREST::Connect_Sck_to_Srvr(const char *ip_addr)
{

    if (inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address / Address not supported\n";
        // return -1;
    }

    // Conectam socketul la serverul de pe localhost
    if (connect(this->_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Connection failed\n";
        // return -1;
    }
}

std::string W_FrREST::Sent(const std::string &request)
{
    // trimitem requestul
    send(this->_socket, request.c_str(), request.length(), 0);
    std::cout << "HTTP request sent\n";

    buffer[1024] = {0};

    // Citim raspunsul serverului
    int valread = read(this->_socket, buffer, 1024);

    close(this->_socket);

    return buffer;
}

json stringToJson(const std::string &jsonString)
{
    try
    {
        // Parsează string-ul în JSON
        json jsonObj = json::parse(jsonString);
        return jsonObj;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Eroare la parsarea JSON-ului: " << e.what() << std::endl;
        return nullptr;
    }
}

// Funcția care transformă JSON în string
std::string jsonToString(const json &jsonObj)
{
    try
    {
        // Serializarea obiectului JSON într-un string
        return jsonObj.dump();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Eroare la serializarea JSON-ului: " << e.what() << std::endl;
        return "";
    }
}

// Funcția care construiește un request HTTP
std::string createHttpRequest(const std::string &method, const std::string &uri, const std::string &http_version,
                              const std::map<std::string, std::string> &headers, const json &jsonBody = {})
{
    std::ostringstream request;

    // 1. Linia de start: METHOD URI HTTP_VERSION
    request << method << " " << uri << " " << http_version << "\r\n";

    // 2. Headerele
    for (const auto &header : headers)
    {
        request << header.first << ": " << header.second << "\r\n";
    }

    // 3. Linia goală care indică sfârșitul headerelor
    request << "\r\n";

    // 4. Corpul (dacă există)
    if (!jsonBody.empty())
    {
        std::string body = jsonToString(jsonBody);
        request << body;
    }

    return request.str();
}

std::string W_FrREST::get(const std::string &uri, const json &request = json::object())
{
    std::map<std::string, std::string> headers = {
        {"Host", "catfact.ninja"},
        {"Content-Type", "application/json"},
        {"Content-Length", "0"}};

    std::string httpRequest = createHttpRequest("GET", uri, "HTTP/1.1", headers, request);
    std::string response = this->Sent(httpRequest);

    return response;
}

std::string W_FrREST::post(const std::string &uri, const json &request)
{
    std::map<std::string, std::string> headers = {
        {"Host", "localhost"},
        {"Content-Type", "application/json"},
        {"Content-Length", "27"}};

    std::string httpRequest = createHttpRequest("POST", uri, "HTTP/1.1", headers, request);
    std::string response = this->Sent(httpRequest);

    return response;
}

std::string W_FrREST::put(const std::string &uri, const json &request)
{
    std::map<std::string, std::string> headers = {
        {"Host", "localhost"},
        {"Content-Type", "application/json"},
        {"Content-Length", "27"}};

    std::string httpRequest = createHttpRequest("PUT", uri, "HTTP/1.1", headers, request);
    std::string response = this->Sent(httpRequest);

    return response;
}

std::string W_FrREST::patch(const std::string &uri, const json &request)
{
    std::map<std::string, std::string> headers = {
        {"Host", "localhost"},
        {"Content-Type", "application/json"},
        {"Content-Length", "27"}};

    std::string httpRequest = createHttpRequest("PATCH", uri, "HTTP/1.1", headers, request);
    std::string response = this->Sent(httpRequest);

    return response;
}

std::string W_FrREST::_delete(const std::string &uri, const json &request)
{
    std::map<std::string, std::string> headers = {
        {"Host", "localhost"},
        {"Content-Type", "application/json"},
        {"Content-Length", "27"}};

    std::string httpRequest = createHttpRequest("DELETE", uri, "HTTP/1.1", headers, request);
    std::string response = this->Sent(httpRequest);

    return response;
}
