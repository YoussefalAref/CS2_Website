#pragma once
#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>
#include <string>
#include <filesystem>
#include "crow.h"
#include "post.h"
#include "User.h"
#include <map>
#include <sstream

using namespace std;

class file{
    private:
        filesystem::path getFilePath() const {
        return filesystem::absolute("posts.csv");
        }
    public:
        // Retrieves all posts from the CSV file. Each post includes authorId, postId, author, content, likesNo.
        vector<Post> retrieveData() const;
        // Writes all posts to the CSV file, including all attributes.
        void fillData(const vector<Post>& posts);
        // Appends a single post to the CSV file, including all attributes.
        void insertData(const Post& p);
        map<int, User> retrieveUsers() const;
        void fillUsers(const map<int, User>& users);
        void insertUser(const User& user);

        file(){}
};