#pragma once
#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>
#include <string>
#include <filesystem>
#include "crow.h"
#include "post.h"
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
        file(){}
};