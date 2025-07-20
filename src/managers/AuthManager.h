#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
#include "crow.h"

struct User {
    std::string username;
    size_t hashedPassword;

    User(const std::string& uname, const std::string& password);
    static size_t hashPassword(const std::string& password);
    bool checkPassword(const std::string& password) const;
};

class AuthManager {
private:
    std::unordered_map<std::string, User> users;
    std::unordered_map<std::string, std::string> sessions;
    std::mutex data_mutex;
    std::string generateToken();
public:
    bool registerUser(const std::string& username, const std::string& password);
    std::string token_authentication(const crow::request& req);
    std::string loginUser(const std::string& username, const std::string& password);
    std::string* getUsernameFromToken(const std::string& token);
};
