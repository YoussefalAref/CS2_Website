#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <unordered_set>
#include <memory>
#include <chrono>
#include "AVLTree.h"
#include "post.h"

class PostManager;
class FriendshipManager;

class User {
private:
    int userId;
    std::string username;
    size_t hashedPassword;
    
    // Friends storage - using unordered_set for O(1) lookup
    std::unordered_set<std::string> friendsSet;
    
    // AVL tree for sorted friend display
    AVLTree friendsAVL;
    

    static size_t hashPassword(const std::string& password);

public:
    PostManager* postmanager;
    FriendshipManager* friendshipmanager;
    
    User(int _userId, const std::string& uname, const std::string& password);
    User(int _userId, const std::string& uname, size_t passwordHash);
    User();
    ~User();
    
    // Getters
    std::string getUsername() const;
    size_t gethashedPassword() const;
    int getUserId() const;
    
    // Password management
    bool checkPassword(const std::string& password) const;
    
    // Friend management
    bool addFriend(const std::string& friendUsername);
    bool removeFriend(const std::string& friendUsername);
    bool isFriend(const std::string& friendUsername) const;
    std::vector<std::string> getFriendsList() const;
    int getFriendsCount() const;
    
    // For database persistence
    void setFriendsList(const std::vector<std::string>& friends);
    std::string getFriendsAsString() const; // For database storage
    void loadFriendsFromString(const std::string& friendsStr); // For database retrieval
};

#endif