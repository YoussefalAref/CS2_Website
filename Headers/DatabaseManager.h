#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include "post.h"
#include "User.h"

class file {
public:
    // Post methods
    std::vector<Post> retrieveData() const;
    void fillData(const std::vector<Post>& posts);
    void insertData(const Post& post);
    
    // User methods
    std::unordered_map<std::string, User> retrieveUsers() const;
    void fillUsers(const std::unordered_map<std::string, User>& users);
    void insertUser(const User& user);
    void updateUser(const User& user); // New method for updating existing user
};

#endif