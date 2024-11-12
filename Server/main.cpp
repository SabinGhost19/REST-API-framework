#include <iostream>
#include "HTTPServer.h"
#include "Utils.h"
#include "./models/PostgresDB.h"
#define PORT 8081
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

void functieptGET(Request &req, Response &res)
{
    std::string body = req.GetBody();
    std::cout << body << std::endl;
    std::string r = req.GetMethod();
    std::cout << "Developer Function: \nMethod : " << r << "\n Body : " << body << std::endl;

    json jsonBody = {
        {"username", "john"},
        {"password", "1234"}};

    res.SetStatusCode(202);
    res.Connection_Type(ConnectionType::Close);
    res.Content_Type(ContentType::ApplicationJson);
    res.Send(jsonBody);
    // res.SetStatusCode(202);
    // res.Connection_Type(ConnectionType::Close);
    // res.Content_Type(ContentType::TextPlain);
    // res.Send("mama are mere const char***");

    // std::string string_my = "server tring....";
    // res.Send(string_my);
}

void functionForSendingFile(Request &req, Response &res)
{
    if (req.GetMethod() == "GET" && req.GetPath() == "/htmlFile")
    {

        res.Content_Type(ContentType::TextHtml);
        res.SendFile("file.html");
        return;
    }

    res.SetStatusCode(400);
    res.Connection_Type(ConnectionType::Close);
    res.Content_Type(ContentType::ApplicationJson);
    json jsonBody = {
        {{"message", "Error at finding and sending the html page"}}};

    res.Send(jsonBody);
}
void functieAna(Request &req, Response &res)
{
    res.Content_Type(ContentType::TextPlain);

    res.Connection_Type(ConnectionType::Close);
    res.SetStatusCode(202);

    res.Send("Ana sunt eu");
}
void functionMiddleWare(Request &req, Response &res, std::function<void()> next)
{
    std::cout << "Middleware: Received a " << req.GetMethod() << " request for " << req.GetPath() << std::endl;
    next();
}
int main()
{
    // Apache Benchmark -testing command
    // ab -n 100 -c 10 http://localhost:8081/home
    // ab -n 100 -c 2 -s 60 http://localhost:8081/home
    //...not working...????????

    // httperf:
    // httperf --server localhost --port 8081 --uri /home --num-conns 1500 --rate 50
    //----meaning: 50 pe second

    // classic testing
    // curl http://localhost:8081/home

    //
    // am mai inceput si trebuie continuat:
    // testare cereri direct din browser
    //
    //
    // FUTURE IDEEAS:
    // redis sau propriul sistem de cache????
    // https://github.com/sewenew/redis-plus-plus
    // client redis++ repo ----

    //  prioritizare  utilizatori autentificati sau cu rol premium
    //---stocare nr cereri per ip sau token pentru asta????
    //
    // routing flexibil si dinamic folosind regex???
    //
    // advanced logging sistem!!!!!!!

    /// suport pentru task scheduling si background jobs?????
    //


    //TESTING CONNECTING TO THE POSTGRES CONTAINER!!!!!!!!!!!!!!!!!!!
    std::string conn_info = "host=localhost port=5431 dbname=REST_API_FRCPP user=sabin password=155015";

    // Crearea obiectului de bază de date și conectarea
    PostgresDB db(conn_info);
    if (!db.connect()) {
        return 1;  // Dacă nu ne putem conecta la baza de date, încheiem execuția
    }

    // Executarea unei interogări simple
    std::string create_table_query = "CREATE TABLE IF NOT EXISTS test_table (id SERIAL PRIMARY KEY, name VARCHAR(50), age INTEGER);";
    if (!db.executeQuery(create_table_query)) {
        return 1;  // Încheiem execuția în caz de eroare
    }

    std::string insert_query = "INSERT INTO test_table (name, age) VALUES ('John Doe', 30);";
    if (!db.executeQuery(insert_query)) {
        std::cerr << "Failed to insert data into the table." << std::endl;
        return 1;  // Încheiem execuția în caz de eroare la inserare
    } else {
        std::cout << "Data inserted successfully." << std::endl;
    }
    // Executarea unei interogări de tip SELECT și afișarea rezultatelor
    std::vector<std::map<std::string, std::string>> results = db.getQueryResults("SELECT * FROM test_table;");
    for (const auto &row : results) {
        for (const auto &[column, value] : row) {
            std::cout << column << ": " << value << std::endl;
        }
        std::cout << "-----------------" << std::endl;
    }

    // Deconectarea de la baza de date
    db.disconnect();





    Router *router = new Router();
    router->addRoute("GET", "/home", functieptGET);
    router->addRoute("GET", "/htmlFile", functionForSendingFile);
    router->addRoute("GET", "/dateAna", functieAna);

    RestServer server(PORT, 5);
    server.addRouter(router);
    


    //middwares are called sequentially
    //it works as expected
    server.use([](Request &req, Response &res, std::function<void()> next)
               {
       std::cout << "Middleware: Received a " << req.GetMethod() << " request for " << req.GetPath() << std::endl;
        next(); });

    server.use(functionMiddleWare);
    // server.use(functieptGET_2);

    server.run();

    return 0;
}