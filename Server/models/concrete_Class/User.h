#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    int id;
    std::string name;
    std::string email;
    std::string password;

    User(int id_, const std::string &name_, const std::string &email_,const std::string&password_) : id(id_), name(name_), email(email_) ,password(password_){}
};

#endif 