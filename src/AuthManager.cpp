#include "AuthManager.h"
#include "DatabaseManager.h"
#include <random>
#include <sstream>

AuthManager::AuthManager(){
    users = dbManager.retrieveUsers(); // This now loads users with their friends
    cout << users.size(); // for debugging
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

bool AuthManager::registerUser(const std::string& uname, const std::string& password) {
    std::lock_guard<std::mutex> lock(data_mutex);
    if (users.count(uname)) return false;
    
    int newId = users.size()+1;
    users.try_emplace(uname, newId, uname, password);
    
    file dbManager;
    dbManager.insertUser(users[uname]); // Save the new user to the database
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
    if (it == users.end() || !it->second.checkPassword(password)){
        return "";
    }
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

// Add this method to get a user reference for friend operations
User* AuthManager::getUser(const std::string& username) {
    std::lock_guard<std::mutex> lock(data_mutex);
    auto it = users.find(username);
    if (it != users.end()) {
        return &it->second;
    }
    return nullptr;
}

// Add this method to update user in memory after friend changes
void AuthManager::updateUserInMemory(const User& user) {
    std::lock_guard<std::mutex> lock(data_mutex);
    users[user.getUsername()] = user;
}

AuthManager::~AuthManager() {
    std::lock_guard<std::mutex> lock(data_mutex);
    dbManager.fillUsers(users); // Save all users to the database on shutdown
}