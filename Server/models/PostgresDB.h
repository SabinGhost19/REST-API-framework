#ifndef POSTGRES_DB_H
#define POSTGRES_DB_H

#include <libpq-fe.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>

class PostgresDB {
public:
    PostgresDB(const std::string& conn_info);
    ~PostgresDB();

    bool connect();
    void disconnect();

    bool executeQuery(const std::string& query);
    std::vector<std::map<std::string, std::string>> getQueryResults(const std::string& query);

private:
    std::string connection_info;
    PGconn *conn;
    void checkConnection();
};

#endif // POSTGRES_DB_H
