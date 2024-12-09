// #ifndef POSTGRES_DB_H
// #define POSTGRES_DB_H

// #include <libpq-fe.h>
// #include <string>
// #include <vector>
// #include <map>
// #include <memory>
// #include <iostream>



// class PostgresDB {
// public:
//     static PostgresDB*getInstance(){
//         if(PostgresDB::instance==NULL){
//             std::cerr<<"PostgresDB wasn't initialize with conn string first";
//             exit(1);
//         }
//     return instance;
//     }
//     static PostgresDB*InitInstance(std::string conn){
//         if(PostgresDB::instance==NULL){
//             PostgresDB::instance=new PostgresDB(conn);
//         }
//     return instance;
//     }

//     PostgresDB(const PostgresDB&)=delete;
//     PostgresDB(const PostgresDB&&)=delete;
//     PostgresDB& operator=(const PostgresDB&) = delete;

//     ~PostgresDB();

//     bool connect();
//     void disconnect();
//     bool executeQuery(const std::string& query);
//     std::vector<std::map<std::string, std::string>> getQueryResults(const std::string& query);

// private:
//     PostgresDB(const std::string& conn_info);
//     std::string connection_info;
//     PGconn *conn;
//     void checkConnection();
//     inline static  PostgresDB* instance=NULL;
// };

// #endif // POSTGRES_DB_H



#ifndef POSTGRES_DB_H
#define POSTGRES_DB_H

#include <libpq-fe.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <mutex>
#include"IGenericDatabase.h"

class PostgresDB : public IGenericDatabase<PGconn>{
public:

    bool connect()override;
    void disconnect()override;
    bool executeQuery(const std::string& query)override;
    std::vector<std::map<std::string, std::string>> getQueryResults(const std::string& query)override;

    static std::unique_ptr<PostgresDB> create(const std::string& conn_info) {
        return std::unique_ptr<PostgresDB>(new PostgresDB(conn_info));
    }

    static PostgresDB& getInstance(const std::string& conn_info = "") {
        static std::once_flag initInstanceFlag;
        std::call_once(initInstanceFlag, [&]() {
            if (conn_info.empty()) {
                throw std::runtime_error("Conexiunea la baza de date nu a fost inițializată corect.");
            }
            instance.reset(new PostgresDB(conn_info));
        });

        if (!instance) {
            throw std::runtime_error("Instanța PostgresDB nu a fost inițializată.");
        }

        return *instance;
    }
    
    PGconn*getConnection();
 
    PostgresDB(const PostgresDB&) = delete;
    PostgresDB(PostgresDB&&) = delete;
    PostgresDB& operator=(const PostgresDB&) = delete;

    ~PostgresDB() {
        disconnect();
    }


private:

    PostgresDB(const std::string& conn_info) : connection_info(conn_info), conn(nullptr) {
        //connect();
    }
    std::string connection_info;
    PGconn *conn;

    void checkConnection();
    inline static std::unique_ptr<PostgresDB> instance = nullptr;
};

#endif 