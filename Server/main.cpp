#include <iostream>
#include "HTTPServer.h"
#include "Utils.h"
#include "./models/PostgresDB.h"
#include"models/DataBaseFactory.h"
#include"models/PostgresDB.h"
#include"repository/userRepository/UserRepository.h"
#include "bcrypt/bcrypt.h"


#define PORT 8082

namespace MyRepos {
    std::shared_ptr<UserRepository> user_repo;  // Declare the global repo object
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
    res.Send("IT WORKS");
}



static int id_nr=0;
void functieIDParamBIG(Request &req,Response&res){
    std::string id = req.GetRouteParam("id");

    std::string response_body = "Requested data for ID: " + id;
    res.SetStatusCode(200);
    res.Content_Type(ContentType::TextPlain);
    id_nr++;
    std::cout<<"FUnctionerasd..."<<id_nr<<std::endl;
    res.Send("--FUNCTIONEAZA.....");
} 



void LoginHandler(Request&req,Response&res){
    std::cout<<req.GetBody()<<"SI AUTH: "<<req.GetHeader("Authorization")<<" Lungime: "<<req.GetHeader("Content-Length")<<std::endl;
    res.Send("--FUNCTIONEAZA.....");
}



void APIEndpointHandler(Request&req,Response&res){
    res.Send("--FUNCTIONEAZA.....API.....");
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


std::string HashPassword(const std::string& password) {
    char salt[BCRYPT_HASHSIZE];  
    char hashed_password[BCRYPT_HASHSIZE];  

    if (bcrypt_gensalt(12, salt) != 0) { 
        std::cerr << "Error generating salt." << std::endl;
        return "";  
    }

    if (bcrypt_hashpw(password.c_str(), salt, hashed_password) != 0) {
        std::cerr << "Error hashing password." << std::endl;
        return "";  
    }

    return std::string(hashed_password);
}

void RegisterHandler(Request &req, Response &res){
    
        std::string body=req.GetBody();
        json body_json=json::parse(body);
        std::cout<<"Json primit prin POST"<<body_json<<std::endl;


        std::string name=body_json["name"];
        std::string email=body_json["email"];
        std::string password=body_json["password"];

        std::string hashed_password=HashPassword(password);

        if (MyRepos::user_repo->emailExists(email)) {
            std::cout << "Email already exists in the database." << std::endl;
        } else {
            std::cout << "Email does not exist in the database." << std::endl;
        }


        User new_user= User(3,name,email,hashed_password);
        try{
            if(MyRepos::user_repo->add(new_user)){
                    std::cout << "User added to the database!!!" << std::endl;
            }
        }catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
            res.SetStatusCode(401);
            res.Send("Internal Server Error");
            exit(1);
        }

        res.SetStatusCode(201);
        res.Send(json({{"message","Added"}}));
}

int main(int argc,char*argv[])
{
    // httperf:
    // httperf --server localhost --port 8081 --uri /home --num-conns 1500 --rate 50
    //----meaning: 50 pe second

    
    //PGconn specify the type of connection to the DataBase
    std::string conn_info = "host=localhost port=5431 dbname=REST_API_FRCPP user=sabin password=155015";
    auto my_data_base=DatabaseFactory::createDatabase(DataBase_Type::Postgres,conn_info);

    if(!my_data_base->connect()){
        return 1;
    }
     
    MyRepos::user_repo = std::make_shared<UserRepository>(std::move(my_data_base));



    //initialize the server
    RestServer server(PORT, 5);

    //init structure project if you want 
    std::string rootPath="./structure";
    server.setupProjectStructure(rootPath);
    
    //initialize the router
    Router *router = new Router();
    //adding routes........
    router->addRoute("GET", "/home", functieptGET);
    router->addRoute("GET", "/htmlFile", functionForSendingFile);
    router->addRoute("GET", "/dateAna", functieAna);
    router->addRoute("GET","/data/:id",functieIDParam);
    router->addRoute("GET","/auth/register",functieIDParamBIG);
    router->addRoute("GET","/login",LoginHandler);

    //good example
    //diff methods same middleware
    server.use("/api",functionMiddleWare);
    router->addRoute("GET","/api",APIEndpointHandler);
    router->addRoute("POST","/api",APIEndpointHandler);
    router->addRoute("PUT","/api",APIEndpointHandler);

    //good example
    router->addRoute("POST","/regiser",RegisterHandler);

    //example 
    router->addRoute("POST","/post",[](Request&req,Response&res){

        std::string body=req.GetBody();
        json body_json=json::parse(body);
        std::cout<<"Json primit prin POST"<<body_json<<std::endl;

        std::string name=body_json["name"];
        std::cout<<"NUmele este...."<<name<<std::endl;

        res.SetStatusCode(200);
        res.Send(json({{"Name",name}}));
    });



    //pass the router with its functionalities to the server
    server.addRouter(router);

    
    //middwares are called sequentially
    server.use([](Request &req, Response &res, std::function<void()> next)
               {
       std::cout << "Middleware: Received a " << req.GetMethod() << " request for " << req.GetPath() << std::endl;
        next(); });
    

    std::vector<std::string>auth_emails;
    auth_emails.push_back("sabinstan19@gmail.com");
    auth_emails.push_back("florentincondur100@gmail.com");
    auth_emails.push_back("test@example.com");

    server.setAuthEmails(auth_emails);
    //ADDED LOGGER FILE for loging the emailss
    server.useSimpleAuthMiddleware("/login");


    server.run();

    PostgresDB::getInstance().disconnect();

    return 0;
}