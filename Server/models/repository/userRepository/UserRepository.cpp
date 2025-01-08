#include "UserRepository.h"
#include <stdexcept>
#include <vector>

bool UserRepository::add(const User &user) {
    const char* query = "INSERT INTO users (name, email, password) VALUES ($1, $2, $3);";
    const char* paramValues[] = {user.name.c_str(), user.email.c_str(), user.password.c_str()};

    PGresult* res = PQexecParams(
        database->getConnection(),
        query,
        3,            // Numărul de parametri
        nullptr,      // Tipurile parametrilor (nullptr pentru inferență)
        paramValues,  // Valorile parametrilor
        nullptr,      // Lungimile stringurilor (nullptr pentru null-terminated strings)
        nullptr,      // Indicatori binari (nullptr pentru text)
        0             // Format text (0 pentru text, 1 pentru binar)
    );

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::string error = PQerrorMessage(database->getConnection());
        PQclear(res);
        throw std::runtime_error("Failed to add user: " + error);
    }

    PQclear(res);
    return true;
}

bool UserRepository::update(const User &user) {
    const char* query = "UPDATE users SET name = $1, email = $2, password = $3 WHERE id = $4;";
    std::string idStr = std::to_string(user.id);
    const char* paramValues[] = {user.name.c_str(), user.email.c_str(), user.password.c_str(), idStr.c_str()};

    PGresult* res = PQexecParams(
        database->getConnection(),
        query,
        4,
        nullptr,
        paramValues,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::string error = PQerrorMessage(database->getConnection());
        PQclear(res);
        throw std::runtime_error("Failed to update user: " + error);
    }

    PQclear(res);
    return true;
}

bool UserRepository::remove(int id) {
    const char* query = "DELETE FROM users WHERE id = $1;";
    std::string idStr = std::to_string(id);
    const char* paramValues[] = {idStr.c_str()};

    PGresult* res = PQexecParams(
        database->getConnection(),
        query,
        1,
        nullptr,
        paramValues,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::string error = PQerrorMessage(database->getConnection());
        PQclear(res);
        throw std::runtime_error("Failed to remove user: " + error);
    }

    PQclear(res);
    return true;
}

User UserRepository::getById(int id) {
    const char* query = "SELECT id, name, email, password FROM users WHERE id = $1;";
    std::string idStr = std::to_string(id);
    const char* paramValues[] = {idStr.c_str()};

    PGresult* res = PQexecParams(
        database->getConnection(),
        query,
        1,
        nullptr,
        paramValues,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::string error = PQerrorMessage(database->getConnection());
        PQclear(res);
        throw std::runtime_error("Failed to fetch user: " + error);
    }

    if (PQntuples(res) == 0) {
        PQclear(res);
        throw std::runtime_error("User not found");
    }

    User user(
        std::stoi(PQgetvalue(res, 0, 0)), // id
        PQgetvalue(res, 0, 1),            // name
        PQgetvalue(res, 0, 2),            // email
        PQgetvalue(res, 0, 3)             // password
    );

    PQclear(res);
    return user;
}

std::vector<User> UserRepository::getAll() {
    const char* query = "SELECT id, name, email, password FROM users;";
    PGresult* res = PQexecParams(
        database->getConnection(),
        query,
        0,       // Fără parametri
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::string error = PQerrorMessage(database->getConnection());
        PQclear(res);
        throw std::runtime_error("Failed to fetch users: " + error);
    }

    std::vector<User> users;
    int nRows = PQntuples(res);

    for (int i = 0; i < nRows; i++) {
        users.emplace_back(
            std::stoi(PQgetvalue(res, i, 0)), // id
            PQgetvalue(res, i, 1),            // name
            PQgetvalue(res, i, 2),            // email
            PQgetvalue(res, i, 3)             // password
        );
    }

    PQclear(res);
    return users;
}

bool UserRepository::emailExists(const std::string &email) {
    const char* query = "SELECT EXISTS(SELECT 1 FROM users WHERE email = $1);";
    const char* paramValues[] = {email.c_str()};

    PGresult* res = PQexecParams(
        database->getConnection(),
        query,
        1,
        nullptr,
        paramValues,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::string error = PQerrorMessage(database->getConnection());
        PQclear(res);
        throw std::runtime_error("Failed to check email existence: " + error);
    }

    bool exists = std::string(PQgetvalue(res, 0, 0)) == "t";

    PQclear(res);
    return !exists;
}



bool UserRepository::exists(int id) {
    std::string query = "SELECT EXISTS(SELECT 1 FROM users WHERE id = $1);";
    std::vector<const char *> paramValues = {std::to_string(id).c_str()};

    PGresult *res = PQexecParams(
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
        throw std::runtime_error("Failed to check user existence: " + std::string(PQerrorMessage(database->getConnection())));
    }

    bool exists = std::string(PQgetvalue(res, 0, 0)) == "t";
    PQclear(res);
    return exists;
}

bool UserRepository::exists(const std::string &field, const std::string &value) {
    std::string query = "SELECT EXISTS(SELECT 1 FROM users WHERE " + field + " = $1);";
    std::vector<const char *> paramValues = {value.c_str()};

    PGresult *res = PQexecParams(
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
        throw std::runtime_error("Failed to check field existence: " + std::string(PQerrorMessage(database->getConnection())));
    }

    bool exists = std::string(PQgetvalue(res, 0, 0)) == "t";
    PQclear(res);
    return exists;
}

std::vector<User> UserRepository::findByField(const std::string &field, const std::string &value) {
    std::string query = "SELECT * FROM users WHERE " + field + " = $1;";
    std::vector<const char *> paramValues = {value.c_str()};

    PGresult *res = PQexecParams(
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
        throw std::runtime_error("Failed to fetch users: " + std::string(PQerrorMessage(database->getConnection())));
    }

    std::vector<User> users;
    int nRows = PQntuples(res);

    for (int i = 0; i < nRows; ++i) {
        users.emplace_back(
            std::stoi(PQgetvalue(res, i, PQfnumber(res, "id"))),
            PQgetvalue(res, i, PQfnumber(res, "name")),
            PQgetvalue(res, i, PQfnumber(res, "email")),
            PQgetvalue(res, i, PQfnumber(res, "password"))
        );
    }

    PQclear(res);
    return users;
}

int UserRepository::count() {
    std::string query = "SELECT COUNT(*) FROM users;";
    PGresult *res = PQexec(database->getConnection(), query.c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        throw std::runtime_error("Failed to count users: " + std::string(PQerrorMessage(database->getConnection())));
    }

    int count = std::stoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    return count;
}

bool UserRepository::removeAll() {
    std::string query = "DELETE FROM users;";
    PGresult *res = PQexec(database->getConnection(), query.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        PQclear(res);
        throw std::runtime_error("Failed to remove all users: " + std::string(PQerrorMessage(database->getConnection())));
    }

    PQclear(res);
    return true;
}

std::vector<User> UserRepository::getAllSorted(const std::string &field, bool ascending) {
    std::string order = ascending ? "ASC" : "DESC";
    std::string query = "SELECT * FROM users ORDER BY " + field + " " + order + ";";
    PGresult *res = PQexec(database->getConnection(), query.c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        throw std::runtime_error("Failed to fetch sorted users: " + std::string(PQerrorMessage(database->getConnection())));
    }

    std::vector<User> users;
    int nRows = PQntuples(res);

    for (int i = 0; i < nRows; ++i) {
        users.emplace_back(
            std::stoi(PQgetvalue(res, i, PQfnumber(res, "id"))),
            PQgetvalue(res, i, PQfnumber(res, "name")),
            PQgetvalue(res, i, PQfnumber(res, "email")),
            PQgetvalue(res, i, PQfnumber(res, "password"))
        );
    }

    PQclear(res);
    return users;
}
