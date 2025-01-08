#include <functional>
#include "Request.h"
#include "Response.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <functional>
#include <fstream> 
#include <ctime>   
#include "Utils.h"

class MiddleWare
{
private:
    std::vector<std::string>vec_of_emails;

    void execute_every_particular_MiddleWare_SbS(size_t index, Request &req, Response &res, std::function<void()> final_handle_of_the_request)
    {
        if (index < this->vector_of_middleWares.size())
        {
            this->vector_of_middleWares[index](req, res, [this, index, &req, &res, final_handle_of_the_request]()
                                               {
                execute_every_particular_MiddleWare_SbS(index + 1, req, res, final_handle_of_the_request); });
        }
        else
        {
            final_handle_of_the_request();
        }
    }

public:
    using MidW_Handler = std::function<void(Request &req, Response &res, std::function<void()>)>;

    std::vector<MidW_Handler> vector_of_middleWares;
    std::map<std::string, MidW_Handler> vector_of_specific_middleWares;


    void setAuthEmails(std::vector<std::string>vec){
        this->vec_of_emails=std::move(vec);
    }
    void use_this_specific_middleWare(std::string endpoint, MidW_Handler mid_function)
    {
        vector_of_specific_middleWares[endpoint] = mid_function;
    };

    void use_this_middleWare(MidW_Handler mid_function)
    {
        this->vector_of_middleWares.push_back(mid_function);
    }

    void execute_middleWares(Request &req, Response &res, std::function<void()> final_handle_of_the_request)
    {
        std::string key = req.GetPath(); 

        std::cout << "Routing key: " << key << std::endl;

        auto it = this->vector_of_specific_middleWares.find(key);
        if (it != this->vector_of_specific_middleWares.end())
        {
           
            it->second(req, res, [this, &req, &res, final_handle_of_the_request]()
                         {
                             execute_every_particular_MiddleWare_SbS(0, req, res, final_handle_of_the_request); });
        }
        else
        {
         
            execute_every_particular_MiddleWare_SbS(0, req, res, final_handle_of_the_request);
        }
    }
    
    void authentication_middleware(Request &req, Response &res, std::function<void()> next)
    {
    
    if (req.GetMethod() == "OPTIONS")
    {
        res.SetHeader("Access-Control-Allow-Origin", "*");
        res.SetHeader("Access-Control-Allow-Methods", "GET, POST, PUT, PATCH, DELETE, OPTIONS");
        res.SetHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
        res.SetStatusCode(200); 
        res.Send("");           
        return;
    }
    std::string auth_header = req.GetHeader("Authorization");

    if (auth_header.empty())
    {
        res.SetStatusCode(401);
        res.Send("Missing Authorization Header");
        return;
    }

    if (auth_header.find("Basic ") == 0)
    {
        std::string encoded = auth_header.substr(6); 

        // Decodificare Base64
        std::string decoded = base64_decode(encoded);

        // Extrage email și parolă
        std::string email, password;
        std::istringstream ss(decoded);
        std::getline(ss, email, ':');
        std::getline(ss, password);

        // Verificare email valid
        if (std::find(vec_of_emails.begin(), vec_of_emails.end(), email) != vec_of_emails.end())
        {
            // Înregistrare autentificare în fișier
            std::ofstream log_file("auth_log.txt", std::ios::app); // Deschide fișierul în modul append
            if (log_file.is_open())
            {
                // Obține timestamp-ul curent
                std::time_t now = std::time(nullptr);
                std::string timestamp = std::asctime(std::localtime(&now));
                timestamp.pop_back(); // Elimină newline-ul generat de asctime

                // Scrie email-ul și timestamp-ul în fișier
                log_file << "Email: " << email << ", Timestamp: " << timestamp << "\n";
                log_file.close();
            }

            // Continuă procesarea următoarelor middleware-uri sau endpoint
            next();
        }
        else
        {
            res.SetStatusCode(401);
            res.Send("Invalid Email");
        }
    }
    else
    {
        res.SetStatusCode(401);
        res.Send("Invalid Authorization Format");
    }
    }
};
