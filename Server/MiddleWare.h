#include <functional>
#include "Request.h"
#include "Response.h"

// am puta zice ca este o clasa pentru stocarea
// executarea si managementul functiilor folosite drept middleware
class MiddleWare
{

private:
    // req,res si functia next pe care o vom defini
    // formatul cum ar trebui sa arate un o functie pentru middleware

    void execute_every_particular_MiddleWare_SbS(size_t index, Request &req, Response &res, std::function<void()> final_handle_of_the_request)
    {
        if (index < this->vector_of_middleWares.size())
        {

            this->vector_of_middleWares[index](req, res, [this, index, &req, &res, final_handle_of_the_request]()
                                               {
                //definirea functiei NEXT() care este o functie lambda
                //rolul de a apela urmatorul middleWare din pool-ul de middleWare-uri existente
                execute_every_particular_MiddleWare_SbS(index+1,req,res,final_handle_of_the_request); });
        }
        else
        {
            // daca nu mai exista middleWare-uri
            // se apeleaza functia de tratare si gestionare a requestului
            // din router...etc...
            final_handle_of_the_request();
        }
    }

public:
    using MidW_Handler = std::function<void(Request &req, Response &res, std::function<void()>)>;
    std::vector<MidW_Handler> vector_of_middleWares;

    void use_this_middleWare(MiddleWare::MidW_Handler mid_function)
    {
        // adaugarea unui nou middleware
        this->vector_of_middleWares.push_back(mid_function);
    }

    // function to initialize the execution of ALL middleWares
    void execute_middleWares(Request &req, Response &res, std::function<void()> final_handle_of_the_request)
    {
        // start execution
        execute_every_particular_MiddleWare_SbS(0, req, res, final_handle_of_the_request);
    }
};