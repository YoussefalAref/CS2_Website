#pragma once
#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>
#include <string>
#include "crow.h"
#include "post.h"
#include "postmanager.h"

class file{
    public:
    vector<Post> retrieveData()const;
    void insertData(const Post& p);
    file(){}
};