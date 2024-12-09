#include <iostream>
#include "HTTPServer.h"
#include "Utils.h"
#include "./models/PostgresDB.h"
#include"models/DataBaseFactory.h"
#include"models/PostgresDB.h"
#include"repository/userRepository/UserRepository.h"

#define PORT 8082
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
void functieIDParam(Request &req,Response&res){
    std::string id = req.GetRouteParam("id");

    std::string response_body = "Requested data for ID: " + id;
    res.SetStatusCode(200);
    res.Content_Type(ContentType::TextPlain);
    res.Send("MERGEEEE");
}
static int id_nr=0;
void functieIDParamBIG(Request &req,Response&res){
    std::string id = req.GetRouteParam("id");

    std::string response_body = "Requested data for ID: " + id;
    res.SetStatusCode(400);
    res.Content_Type(ContentType::TextPlain);
    id_nr++;
    std::cout<<"FUnctionerasd..."<<id_nr<<std::endl;
    res.Send("--FUNCTIONEAZA.....");
}
void LoginHandler(Request&req,Response&res){
    std::cout<<req.GetBody()<<"SI AUTH: "<<req.GetHeader("Authorization")<<" Lungime: "<<req.GetHeader("Content-Length")<<std::endl;
}
void ClassicAuthMiddleware_withPostgres(Request&req,Response&res,std::function<void()>next){
    
    //interface
    //
    //
    std::string body=req.GetBody();

    json body_json=json::parse(body);

    std::string email=body_json["email"];

    std::string query = "SELECT EXISTS(SELECT 1 FROM users WHERE email = '" + email + "');";
     if (PostgresDB::getInstance().executeQuery(query)) {

        res.SetStatusCode(403);
        res.Send("Email already exist");
    }

    next();
}
void functionMiddleWare(Request &req, Response &res, std::function<void()> next)
{
    std::cout << "Middleware: Received a API API API API!! " << req.GetMethod() << " request for " << req.GetPath() << std::endl;
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
    
    //make the connection string
   
    //init the DB providing the string
    // std::string conn_info = "host=localhost port=5431 dbname=REST_API_FRCPP user=sabin password=155015";
    // PostgresDB::getInstance(conn_info);
  
    // if (!PostgresDB::getInstance().connect()) {
    //     return 1; 
    // }

    // std::string query = "TRUNCATE TABLE test_table;";
    // if (!PostgresDB::getInstance().executeQuery(query)) {
    //     std::cerr << "Failed to insert data into the table." << std::endl;
    //     return 1; 
    // } else {
    //     std::cout << "Data delete successfully." << std::endl;
    // }









    std::string conn_info = "host=localhost port=5431 dbname=REST_API_FRCPP user=sabin password=155015";
    auto my_data_base=DatabaseFactory::createDatabase(DataBase_Type::Postgres,conn_info);

    if(!my_data_base->connect()){
        return 1;
    }

    //PGconn specify the type of connection to the DataBase
    auto repo=std::make_shared<UserRepository>(std::move(my_data_base));

    // User new_user=User(2,"sabin","sabinstan19@gmail.com");
    // try{
    //     if(repo->add(new_user)){
    //             std::cout << "User added to the database!!!" << std::endl;
    //     }
    //     return 0;
    // }catch (const std::exception& ex) {
    //     std::cerr << "Error: " << ex.what() << std::endl;
    //     return 1;
    // }


    // std::string email = "sabinstan19@gmail.com";
    //     if (repo->emailExists(email)) {
    //         std::cout << "Email already exists in the database." << std::endl;
    //     } else {
    //         std::cout << "Email does not exist in the database." << std::endl;
    //     }

    //     // Deconectarea bazei de date
    // std::cout<<"Deconnecting from the DataBase....\n";






    //DECONECTAREA SE FACE AUTOMATA
    // LA DESTRUCTOR.... IN REPO care este un UNIQUE_POINTER....
     //!!!!!!!!!!!!!!!!!!!!!!!!!!!1   




    //define some query using a string and call de exec function with it 
//     std::string create_table_query = "CREATE TABLE IF NOT EXISTS test_table (id SERIAL PRIMARY KEY, name VARCHAR(50), age INTEGER);";
//     if (!db.executeQuery(create_table_query)) {
//         return 1; 
//     }

//     std::string insert_query = "INSERT INTO test_table (name, age) VALUES ('John Doe', 30);";
//     if (!db.executeQuery(insert_query)) {
//         std::cerr << "Failed to insert data into the table." << std::endl;
//         return 1; 
//     } else {
//         std::cout << "Data inserted successfully." << std::endl;
//     }
//    //printing the fetched data for testing
//     std::vector<std::map<std::string, std::string>> results = db.getQueryResults("SELECT * FROM test_table;");
//     for (const auto &row : results) {
//         for (const auto &[column, value] : row) {
//             std::cout << column << ": " << value << std::endl;
//         }
//         std::cout << "-----------------" << std::endl;
//     }

    // disconnect from the data base



    //initialize the server
    RestServer server(PORT, 5);
    //initialize the router
    Router *router = new Router();
    //adding some routes........
    router->addRoute("GET", "/home", functieptGET);
    router->addRoute("GET", "/htmlFile", functionForSendingFile);
    router->addRoute("GET", "/dateAna", functieAna);
    router->addRoute("GET","/data/:id",functieIDParam);
    router->addRoute("GET","/auth/register",functieIDParamBIG);
    router->addRoute("GET","/login",LoginHandler);
    
    router->addRoute("POST","/post",[](Request&req,Response&res){

        std::string body=req.GetBody();
        json body_json=json::parse(body);
        std::cout<<"Json primit prin POST"<<body_json<<std::endl;

        std::string name=body_json["name"];
        std::cout<<"NUmele este...."<<name<<std::endl;

        res.SetStatusCode(200);
        res.Send(json({{"Name","Bogdan"}}));
    });
    //apass the router with its functionalities to the server
    server.addRouter(router);

    std::vector<std::string>emails;
    emails.push_back("sabinstan19@gmail.com");
    server.setAuthEmails(emails);
    


    //middwares are called sequentially
    server.use([](Request &req, Response &res, std::function<void()> next)
               {
       std::cout << "Middleware: Received a " << req.GetMethod() << " request for " << req.GetPath() << std::endl;
        next(); });

    server.use("/api",functionMiddleWare);

    std::vector<std::string>auth_emails;
    auth_emails.push_back("sabinstan19@gmail.com");
    auth_emails.push_back("florentincondur100@gmail.com");

    server.setAuthEmails(auth_emails);
    server.useSimpleAuthMiddleware("/login");
    // server.use(functieptGET_2);
    server.run();

    PostgresDB::getInstance().disconnect();

    // sa verific daca merg toate get post put patch delete
    // logger pentru middleware
    //middleware pentru auteitficare
    //in headerul authentification sa iau email si parola
    //sa ii dau un fisier sau un strig middleware-ului si acesta sa poata 
    //verifica daca userul exista sau nu 

    //asta ar trebui sa presupuna middleware format din rute
    //pentru un endpoint specific sa fie un middleware


    
    //sa mai adaug mai multe metode pentur baza de date
    //mai multe metode generice
    //raman la conceptul de IRespository dispus developerului
    //si UserRepository de exemplu este cel facut de mine 
    //!!!!!!!!!!!!!!
    //sa modific neaparat la acele query-uri ale apelului de baza de date
    //stringurile sa nu fie cu in clar
    //si sa fie cu un apel specific ,
    //este o functie in libraria aceea
    //pentru prevenire sql injection 


    //!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //sa fac eu de mana toate thredurile
    //multithreading 
    //sa le fac iar de la 0 si sa fac cu threaduri eu de mana ca in c
    
    //loggers facute pentru fiecare request primit 
    return 0;
}