#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
#include "crow.h"
#include "User.h"
#include "DatabaseManager.h"  // Include DatabaseManager header

class AuthManager {
private:
    std::unordered_map<std::string, std::string> sessions;
    std::mutex data_mutex;
    file dbManager;  // Correct type for database operations
    std::string generateToken();
    
    public:
    std::unordered_map<std::string, User> users;  // Moved to private
    AuthManager();
    ~AuthManager();  // Destructor to save data on shutdown

    bool registerUser(const std::string& uname, const std::string& password);
    std::string token_authentication(const crow::request& req);
    std::string loginUser(const std::string& username, const std::string& password);
    std::string* getUsernameFromToken(const std::string& token);
    
    // User management methods
    User* getUser(const std::string& username);
    void updateUserInMemory(const User& user);
};