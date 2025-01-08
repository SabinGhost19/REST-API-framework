#ifndef IREPOSITORY_H
#define IREPOSITORY_H

#include <string>
#include <vector>
#include <map>

template <typename T>
class IRepository {
public:
    virtual bool add(const T& entity) = 0;
    virtual bool remove(int id) = 0;
    virtual bool update(const T& entity) = 0;
    virtual T getById(int id) = 0;
    virtual std::vector<T> getAll() = 0;
    virtual bool exists(int id) = 0;
    virtual bool exists(const std::string& field, const std::string& value) = 0;
    virtual std::vector<T> findByField(const std::string& field, const std::string& value) = 0;
    virtual int count() = 0;
    virtual bool removeAll() = 0;
    virtual std::vector<T> getAllSorted(const std::string& field, bool ascending = true) = 0;
    // Others...
    // virtual std::vector<T> findByCriteria(const std::map<std::string, std::string>& criteria) = 0;
    // virtual std::vector<T> findByRange(const std::string& field, double min, double max) = 0;
    // virtual std::vector<T> findByDateRange(const std::string& field, const std::string& startDate, const std::string& endDate) = 0;

    virtual ~IRepository() = default;
};

#endif
