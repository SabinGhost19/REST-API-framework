#include "PostgresDB.h"

PostgresDB::PostgresDB(const std::string& conn_info)
    : connection_info(conn_info), conn(nullptr) {}

PostgresDB::~PostgresDB() {
    disconnect();
}

bool PostgresDB::connect() {
    conn = PQconnectdb(connection_info.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        return false;
    }
    std::cout << "Connected to the database successfully." << std::endl;
    return true;
}

void PostgresDB::disconnect() {
    if (conn != nullptr) {
        PQfinish(conn);
        conn = nullptr;
        std::cout << "Disconnected from the database." << std::endl;
    }
}

bool PostgresDB::executeQuery(const std::string& query) {
    checkConnection();
    PGresult *res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Query execution failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

std::vector<std::map<std::string, std::string>> PostgresDB::getQueryResults(const std::string& query) {
    checkConnection();
    PGresult *res = PQexec(conn, query.c_str());
    std::vector<std::map<std::string, std::string>> results;

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Query execution failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return results;
    }

    int rows = PQntuples(res);
    int cols = PQnfields(res);

    for (int i = 0; i < rows; i++) {
        std::map<std::string, std::string> row;
        for (int j = 0; j < cols; j++) {
            std::string column_name = PQfname(res, j);
            std::string value = PQgetvalue(res, i, j);
            row[column_name] = value;
        }
        results.push_back(row);
    }

    PQclear(res);
    return results;
}

void PostgresDB::checkConnection() {
    if (conn == nullptr || PQstatus(conn) != CONNECTION_OK) {
        throw std::runtime_error("Not connected to the database.");
    }
}
