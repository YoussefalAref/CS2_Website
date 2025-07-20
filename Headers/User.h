#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <chrono>
#include <memory>
#include "postmanager.h"
#include "AVLTree.h"

class Post;
class Comment;

class User{
private:
    std::string username;
    std::string email;
    size_t hashedPassword;
    std::chrono::system_clock::time_point lastActive;
    bool isActive;

    AVLTree<std::string> friends;
    
    std::vector<std::shared_ptr<Post>> userPosts;
    std::vector<std::string> pendingRequests;
    std::vector<std::string> sentRequests;
    
    static size_t hashPassword(const std::string& password);
    bool addFriend(const std::string& friendUsername);//for testing user can only send a request
    PostManager postmanager;
    int userId;

public:
    string getEmail() const { return email; }
    User(int _userId, const std::string& uname, const std::string& password, const std::string& userEmail);
    ~User();
    //postmanager has a value that doesnt express user id
    User(): postmanager(0) {} // Default constructor for empty user;
    
    bool checkPassword(const std::string& password) const;
//     void changePassword(const std::string& oldPassword, const std::string& newPassword);

//     void setEmail(const std::string& email);
//     void setLastActive();
//     void setActive(bool active);

//     //getters
//     std::string getUsername() const;
//     std::string getEmail() const;
//     std::chrono::system_clock::time_point getLastActive() const;
//     bool getIsActive() const;
//     size_t getHashedPassword() const;
    
//     //friend management
//     bool removeFriend(const std::string& friendUsername);
//     bool isFriend(const std::string& username) const;
//     std::vector<std::string> getFriendsList() const;
//     int getFriendsCount() const;

//     //friend request managment
//     bool sendFriendRequest(const std::string& toUsername);
//     bool acceptFriendRequest(const std::string& fromUsername);
//     bool rejectFriendRequest(const std::string& fromUsername);
//     bool cancelSentRequest(const std::string& toUsername);
//     void addPendingRequest(const std::string& fromUsername);
//     void removePendingRequest(const std::string& fromUsername);
//     std::vector<std::string> getPendingRequests() const;
//     std::vector<std::string> getSentRequests() const;

//     //Post managment
//     void addPost(std::shared_ptr<Post> post);
//     bool removePost(const int& postId);
//     std::vector<std::shared_ptr<Post>> getUserPosts() const;
//     std::vector<std::shared_ptr<Post>> getRecentPosts(int limit = 10) const;

//     //friend suggestions
//     std::vector<std::string> getMutualFriends(const User& otherUser) const;
//     std::vector<std::string> suggestFriends(const std::vector<User>& allUsers) const;

//     //validations
//     bool isValidUser(const std::string& username) const;
//     bool isValidEmail(const std::string& email) const;

//     //operators
//     bool operator==(const User& other) const;
//     bool operator!=(const User& other) const;

//     // Friend class access
//     friend class FriendshipManager;
//     friend class AuthManager;
//     friend class PostManager;
//     friend class TimelineManager;

//     //translate to and from json
//     std::string toJson() const;
//     static User fromJson(const std::string& jsonStr);
};

#endif
