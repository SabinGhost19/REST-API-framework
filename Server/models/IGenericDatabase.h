#ifndef IGENERICDATABASE_H
#define IGENERICDATABASE_H

#include <string>
#include <vector>
#include <map>
#include <libpq-fe.h>

template<class T>
class IGenericDatabase {

protected:
        T *conn=NULL;
public:
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual bool executeQuery(const std::string &query) = 0;
    virtual T*getConnection()=0;

    virtual std::vector<std::map<std::string, std::string>> getQueryResults(const std::string &query) = 0;
    virtual ~IGenericDatabase() = default;
};

#endif 