#pragma once
#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>
#include "crow.h"
#include "post.h"
#include "postmanager.h"

class file{
    public:
    vector<Post> retrieveData()const;
    void saveData(const Post& p);
    file(){}
};