#include <functional>
#include "Request.h"
#include "Response.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <functional>
//TREBUIE CREATA
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

          
            std::string decoded = base64_decode(encoded);

            
            std::string email, password;
            std::istringstream ss(decoded);
            std::getline(ss, email, ':');
            std::getline(ss, password);

          
            if (std::find(vec_of_emails.begin(), vec_of_emails.end(), email) != vec_of_emails.end())
            {
                
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
