#include <iostream>

class Car
{
    int numar_inm = 0;

public:
    Car(int nr) : numar_inm(nr) {}
    int GetNr() { return this->numar_inm; }
};

void metodaPentru(HTTPRequest &request, HTTPResponse &response)
{
    // implementare....cum vrea developerul
    // ex:

    res.send("Buna,sunt serverul,fac handle pe endpointul /home");
    // sau
    ls

        res.sendCode(404); // crashed
}

int main()
{

    Router router;
    router.get("/home", metodaPentruget);
    router.post("/user", metodaPentruPost);
    HTTPHandler / Server server("adresa de localhost", port);
    server.setRouter(router);

    server.listen();

    return 0;
}