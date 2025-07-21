#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
#include "crow.h"
#include "User.h"


class AuthManager {
private:
    std::unordered_map<std::string, std::string> sessions;
    std::mutex data_mutex;
    std::string generateToken();
public:
    AuthManager();
    std::unordered_map<std::string, User> users; // key is username
    bool registerUser(const std::string& uname, const std::string& password);
    std::string token_authentication(const crow::request& req);
    std::string loginUser(const std::string& username, const std::string& password);
    std::string* getUsernameFromToken(const std::string& token);
};
