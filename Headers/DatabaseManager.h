#pragma once
#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>
#include <string>
#include <filesystem>
#include "crow.h"
#include "post.h"
#include "postmanager.h"
using namespace std;

class file{
    private:
        filesystem::path getFilePath() const {
        return filesystem::absolute("posts.csv");
        }
    public:
        vector<Post> retrieveData()const;
        void fillData(const vector<Post>& post);
        void insertData(const Post& p);
        file(){}
};