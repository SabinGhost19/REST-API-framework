#include "UserRepository.h"
#include <stdexcept>

bool UserRepository::add(const User &user) {
    std::string query = "INSERT INTO users (name, email) VALUES ($1, $2);";
    std::vector<const char*> paramValues = {user.name.c_str(), user.email.c_str()};

    PGresult* res = PQexecParams(
        database->getConnection(),            // Conexiunea către baza de date
        query.c_str(),                 // Query-ul SQL
        paramValues.size(),            // Numărul de parametri
        nullptr,                       // Tipurile parametrilor (nullptr pentru inferență)
        paramValues.data(),            // Valorile parametrilor
        nullptr,                       // Lungimile stringurilor (nullptr pentru null-terminated strings)
        nullptr,                       // Indicatori binari (nullptr pentru text)
        0                              // Format text (0 pentru text, 1 pentru binar)
    );

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        PQclear(res);
        throw std::runtime_error("Failed to add user: " + std::string(PQerrorMessage(database->getConnection())));
    }

    PQclear(res);
    return true;
}


bool UserRepository::update(const User &user) {
    std::string query = "UPDATE users SET name = $1, email = $2 WHERE id = $3;";
    std::vector<const char*> paramValues = {user.name.c_str(), user.email.c_str(), std::to_string(user.id).c_str()};

    PGresult* res = PQexecParams(
        database->getConnection(),
        query.c_str(),
        paramValues.size(),
        nullptr,
        paramValues.data(),
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        PQclear(res);
        throw std::runtime_error("Failed to update user: " + std::string(PQerrorMessage(database->getConnection())));
    }

    PQclear(res);
    return true;
}


bool UserRepository::remove(int id) {
    std::string query = "DELETE FROM users WHERE id = $1;";
    std::vector<const char*> paramValues = {std::to_string(id).c_str()};

    PGresult* res = PQexecParams(
        database->getConnection(),
        query.c_str(),
        paramValues.size(),
        nullptr,
        paramValues.data(),
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        PQclear(res);
        throw std::runtime_error("Failed to remove user: " + std::string(PQerrorMessage(database->getConnection())));
    }

    PQclear(res);
    return true;
}

User UserRepository::getById(int id) {
    std::string query = "SELECT * FROM users WHERE id = $1;";
    std::vector<const char*> paramValues = {std::to_string(id).c_str()};

    PGresult* res = PQexecParams(
        database->getConnection(),
        query.c_str(),
        paramValues.size(),
        nullptr,
        paramValues.data(),
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        throw std::runtime_error("Failed to fetch user: " + std::string(PQerrorMessage(database->getConnection())));
    }

    if (PQntuples(res) == 0) {
        PQclear(res);
        throw std::runtime_error("User not found");
    }

    User user(
        std::stoi(PQgetvalue(res, 0, PQfnumber(res, "id"))),
        PQgetvalue(res, 0, PQfnumber(res, "name")),
        PQgetvalue(res, 0, PQfnumber(res, "email"))
    );

    PQclear(res);
    return user;
}


std::vector<User> UserRepository::getAll() {
    std::string query = "SELECT * FROM users;";
    PGresult* res = PQexecParams(
        database->getConnection(),
        query.c_str(),
        0,       // Fără parametri
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        throw std::runtime_error("Failed to fetch users: " + std::string(PQerrorMessage(database->getConnection())));
    }

    std::vector<User> users;
    int nRows = PQntuples(res);

    for (int i = 0; i < nRows; i++) {
        users.emplace_back(
            std::stoi(PQgetvalue(res, i, PQfnumber(res, "id"))),
            PQgetvalue(res, i, PQfnumber(res, "name")),
            PQgetvalue(res, i, PQfnumber(res, "email"))
        );
    }

    PQclear(res);
    return users;
}
bool UserRepository::emailExists(const std::string &email) {
    std::string query = "SELECT EXISTS(SELECT 1 FROM users WHERE email = $1);";
    std::vector<const char*> paramValues = {email.c_str()};

    PGresult* res = PQexecParams(
        database->getConnection(),
        query.c_str(),
        paramValues.size(),
        nullptr,
        paramValues.data(),
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        throw std::runtime_error("Failed to check email existence: " + std::string(PQerrorMessage(database->getConnection())));
    }

    bool exists = std::string(PQgetvalue(res, 0, 0)) == "t";

    PQclear(res);
    return exists;
}
