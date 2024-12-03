#ifndef IREPOSITORY_H
#define IREPOSITORY_H

#include <string>
#include <vector>
#include <map>

template <typename T>
class IRepository{
public:
    virtual bool add(const T&entity)=0;
    virtual bool remove(int id)=0;
    virtual bool update(const T&entity)=0;
    virtual T getById(int id)=0;
    virtual std::vector<T>getAll()=0;
    virtual ~IRepository() = default;
};

#endif 