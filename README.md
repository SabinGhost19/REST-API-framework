
# 🚀 REST API Framework in C++

A modern, developer-friendly framework for creating scalable and efficient RESTful APIs in C++. Designed with modularity and flexibility, this framework simplifies backend development by abstracting core functionalities such as routing, middleware, and database integration.

---

## ✨ Features

- **Dynamic Routing**: Flexibly handle fixed and parameterized routes.
- **Middleware Support**: Easily integrate middleware for authentication, logging, and more.
- **Secure Password Handling**: Powered by `bcrypt` for hashing and verifying passwords.
- **Database Abstraction**: Generic repository pattern with PostgreSQL and MySQL support.
- **Project Automation**: Automatically generate the project structure, including migrations, models, repositories, and tests.
- **Scalable HTTP Server**: Efficiently handle client requests and responses with `HTTPServer`.

---

## 📖 Table of Contents

- [Motivation](#motivation)
- [Project Goals](#project-goals)
- [Core Components](#core-components)
- [Dynamic Routing](#dynamic-routing)
- [Middleware Functionality](#middleware-functionality)
- [Password Encryption](#password-encryption)
- [Automated Project Setup](#automated-project-setup)
- [Repository Pattern](#repository-pattern)
- [Getting Started](#getting-started)
- [Class Diagram](#class-diagram)
- [Future Scope](#future-scope)

---

## 💡 Motivation

The **REST API Framework in C++** aims to provide:
- Abstraction of core functionalities to reduce boilerplate code
- Standardization and modularity for consistent development practices
- Flexibility and scalability to meet diverse application requirements

---

## 🎯 Project Goals

- Simplify the development of RESTful APIs in C++
- Ensure scalability, modularity, and reusability
- Abstract complex functionalities like database access and routing

---

## 🛠️ Core Components

### **HTTPServer**
The core server for handling HTTP requests and responses

### **Router**
Dynamically maps routes to handler functions with support for both fixed and parameterized paths

### **Middleware**
Enables pre-processing of requests, such as authentication, logging, or input validation

### **PostgresDB**
Database abstraction layer for seamless integration with PostgreSQL

---

## 🔀 Dynamic Routing

### Functionality:
Dynamic routes allow handling paths with parameters effortlessly. For example:
```cpp
router->addRoute("GET", "/data/:id", handleDataByID);
router->addRoute("POST", "/users", createUser);
```
**Benefits**:
- Flexible mapping of URLs to handler functions.
- Easily manage static and dynamic routes.

---

## 🛡️ Middleware Functionality

### Example: Basic Authentication
Protect your routes using `Base64`-encoded credentials.
```cpp
std::vector<std::string> auth_emails = {"admin@example.com", "user@example.com"};
server.setAuthEmails(auth_emails);
server.useSimpleAuthMiddleware("/protected-route");
```
**Features**:
- Secure sensitive routes with minimal effort.
- Includes logging for authentication attempts.

---

## 🔒 Password Encryption

Leverages `bcrypt` to hash and verify passwords securely.
```cpp
std::string hashed_password = HashPassword("securePassword123");
```
**Advantages**:
- Protect sensitive user data against brute force and dictionary attacks.
- Industry-standard encryption for secure authentication systems.

---

## 📁 Automated Project Setup

The framework generates a structured project with essential directories and files:
- **migrations/**: SQL scripts for database schema.
- **models/**: Data models such as `User.h`.
- **repository/**: Repositories for database operations.
- **config/**: Configuration files (e.g., `database.json`).
- **logs/**: Logging directory for application logs.
- **tests/**: Directory for unit and integration tests.

---

## 💾 Repository Pattern

A generic `IRepository` interface defines common database operations:
```cpp
template <typename T>
class IRepository {
public:
    virtual bool add(const T& entity) = 0;
    virtual bool remove(int id) = 0;
    virtual T getById(int id) = 0;
    virtual std::vector<T> getAll() = 0;
    virtual bool exists(int id) = 0;
    virtual bool exists(const std::string& field, const std::string& value) = 0;
    virtual std::vector<T> findByField(const std::string& field, const std::string& value) = 0;
    virtual int count() = 0;
    virtual bool removeAll() = 0;
    virtual std::vector<T> getAllSorted(const std::string& field, bool ascending = true) = 0;
};
```

First instantiate a database connection using the DatabaseFactory. 
This factory allows the selection of a database type (e.g., PostgreSQL, MySQL) 
and creates a connection based on configuration parameters.

Developers should derive their own concrete repository class (e.g., UserRepository, ItemsRepository etc...) from IRepository. 
This class can leverage the predefined methods of IRepository 
and extend functionality with additional methods specific to their application's needs.

### Example: User Repository
Concrete repository implementation for managing user data:
```cpp
    std::string conn_info = "host=HOST_NAME port=PORT_NR dbname=REST_API_FRCPP user=USER password=PASSWORD;
    auto my_data_base=DatabaseFactory::createDatabase(DataBase_Type::Postgres,conn_info);

    if(!my_data_base->connect()){
        return 1;
    }


    //creating a namespace:
    namespace MyRepos {
        std::shared_ptr<UserRepository> user_repo;  // Declare the global repo object
    }
    
    MyRepos::user_repo = std::make_shared<UserRepository>(std::move(my_data_base));


    //OR:
    auto db = DatabaseFactory::createDatabase(DataBase_Type::Postgres, connectionInfo);
    UserRepository userRepo(std::move(db));
```

---

## 🛠️ Getting Started

### Prerequisites
- C++17 or higher.
- PostgreSQL or MySQL for database integration.

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/username/rest-api-framework-cpp.git
   ```
2. Navigate to the project directory and build:
   ```bash
   cd rest-api-framework-cpp
   make
   ```
3. Run the server:
   ```bash
   ./main
   ```

---

## 📊 Class Diagram


![image](https://github.com/user-attachments/assets/33bcb900-7554-4e66-a19b-15ff654bc8eb)


---

## 🚀 Future Scope

- Expand database support (e.g., MongoDB).
- Add more middleware options like rate-limiting and caching.
- Enhance logging and monitoring features.

---

