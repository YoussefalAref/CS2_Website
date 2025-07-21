#pragma once
#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>
#include <string>
#include <filesystem>
#include <unordered_map>
#include "post.h"

class User;


using namespace std;

class file {
public:
    // Post operations
    vector<Post> retrieveData() const;
    void fillData(const vector<Post>& posts);
    void insertData(const Post& p);

    // User operations (using unordered_map)
    unordered_map<string, User> retrieveUsers() const;
    void fillUsers(const unordered_map<string, User>& users);
    void insertUser(const User& user);

    file(){};
};