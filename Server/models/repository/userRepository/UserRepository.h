#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "../IRepository.h"
#include "IGenericDatabase.h"
#include "../../concrete_Class/User.h"
#include <memory>

class UserRepository : public IRepository<User> {
private:
    std::shared_ptr<IGenericDatabase<PGconn>> database;

public:
    explicit UserRepository(std::shared_ptr<IGenericDatabase<PGconn>> db) : database(std::move(db)) {}
    
    bool add(const User &user) override; 
    bool update(const User &user) override; 
    bool remove(int id) override ;
    User getById(int id) override; 
    std::vector<User> getAll() override; 
    bool exists(int id)override;
    bool exists(const std::string &field, const std::string &value)override;
    std::vector<User> findByField(const std::string &field, const std::string &value);
    int count();
    bool removeAll();
    std::vector<User> getAllSorted(const std::string &field, bool ascending); 

    
    bool emailExists(const std::string &email);
};

#endif 