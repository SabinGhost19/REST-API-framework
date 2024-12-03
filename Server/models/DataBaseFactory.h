#ifndef DATABASEFACTORY_H
#define DATABASEFACTORY_H

#include "IGenericDatabase.h"
#include"PostgresDB.h"
#include <memory>
#include"Utils.h"

class DatabaseFactory {
public:
    static std::unique_ptr<IGenericDatabase> createDatabase(const DataBase_Type &type, const std::string &conn_info) {
        
            switch(type){
                case DataBase_Type::Postgres:
                {
                    return std::make_unique<PostgresDB>(conn_info);
                    break;
                }
                case DataBase_Type::MySQL:
                {
                    //to be implemented
                    break;
                }
                default:{
                     throw std::runtime_error("Unsupported database type");
                }
            }
          
    }
};

#endif 