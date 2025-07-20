#include "AuthManager.h"
#include <random>
#include <sstream>

User::User(const std::string& uname, const std::string& password)
    : username(uname), hashedPassword(hashPassword(password)) {}

size_t User::hashPassword(const std::string& password) {
    return std::hash<std::string>{}(password);
}

bool User::checkPassword(const std::string& password) const {
    return hashedPassword == hashPassword(password);
}

std::string AuthManager::generateToken() {
    std::stringstream ss;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    for (int i = 0; i < 32; ++i)
        ss << std::hex << dis(gen);
    return ss.str();
}

bool AuthManager::registerUser(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(data_mutex);
    if (users.count(username)) return false;
    users.emplace(username, User(username, password));
    return true;
}

std::string AuthManager::token_authentication(const crow::request& req) {
    std::string cookie_header = req.get_header_value("Cookie");
    std::string key = "token=";
    std::string auth_token;
    size_t start = cookie_header.find(key);
    if (start != std::string::npos) {
        start += key.length();
        size_t end = cookie_header.find(";", start);
        auth_token = cookie_header.substr(start, end - start);
    }
    return auth_token;
}

std::string AuthManager::loginUser(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(data_mutex);
    auto it = users.find(username);
    if (it == users.end() || !it->second.checkPassword(password))
        return "";

    std::string token = generateToken();
    sessions[token] = username;
    return token;
}

std::string* AuthManager::getUsernameFromToken(const std::string& token) {
    std::lock_guard<std::mutex> lock(data_mutex);
    auto it = sessions.find(token);
    if (it != sessions.end()) {
        return &it->second;
    }
    return nullptr;
}
