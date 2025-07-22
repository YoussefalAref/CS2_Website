#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
class User;
class AVLTree {
private:
    struct Friend{
        std::string name;
        Friend* left;
        Friend* right;
        int height;
        Friend(const std::string& n):name(n),left(nullptr), right(nullptr), height(1){}
    };
    public:
    Friend* root;
    int balanceFactor(Friend* node) const;
    int getHeight(Friend* node) const;
    void newHeight(Friend* node);
    Friend* rotateRight(Friend* y);
    Friend* rotateLeft(Friend* x);
    Friend* balance(Friend* node);
    AVLTree() : root(nullptr) {}
    AVLTree(const vector<string>& friends){
        for (const auto& friendName : friends) {
            root = insert(root, friendName);
        }
    }
    ~AVLTree(){destroyTree(root);}
    void destroyTree(Friend* node);
    Friend* insert(Friend* node, const std::string& name);
    Friend* search(Friend* node, const std::string& name) const;
    void addFriend(const std::string& name);
    Friend* removeFriend(Friend* root,const std::string& name);
    bool isfriend(const std::string& name) const;
    void displayFriends(Friend* node) const;
    void showAllFriends() const;
    Friend* findMin(Friend* node);
};


