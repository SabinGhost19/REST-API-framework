#include "UserRepository.h"


    bool UserRepository::add(const User &user)  {
        std::string query = "INSERT INTO users (name, email) VALUES ('" + user.name + "', '" + user.email + "');";
        return database->executeQuery(query);
    }

    bool UserRepository::update(const User &user)  {
        std::string query = "UPDATE users SET name = '" + user.name + "', email = '" + user.email + "' WHERE id = " + std::to_string(user.id) + ";";
        return database->executeQuery(query);
    }

    bool UserRepository::remove(int id)  {
        std::string query = "DELETE FROM users WHERE id = " + std::to_string(id) + ";";
        return database->executeQuery(query);
    }

    User UserRepository::getById(int id)  {
        std::string query = "SELECT * FROM users WHERE id = " + std::to_string(id) + ";";
        auto results = database->getQueryResults(query);

        if (!results.empty()) {
            auto &row = results[0];
            return User(std::stoi(row["id"]), row["name"], row["email"]);
        }

        //throw std::runtime_error("User not found");
    }

    std::vector<User> UserRepository::getAll()  {
        std::string query = "SELECT * FROM users;";
        auto results = database->getQueryResults(query);

        std::vector<User> users;
        for (const auto &row : results) {
            users.emplace_back(std::stoi(row.at("id")), row.at("name"), row.at("email"));
        }

        return users;
    }

    bool UserRepository::emailExists(const std::string &email) {
        std::string query = "SELECT EXISTS(SELECT 1 FROM users WHERE email = '" + email + "');";
        auto results = database->getQueryResults(query);
        return !results.empty() && results[0]["exists"] == "t";
    }



