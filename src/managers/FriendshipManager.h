#ifndef FRIENDSHIPMANAGER_H
#define FRIENDSHIPMANAGER_H

#include <string>
#include <vector>
#include <memory>
#include "../models/User.h"
#include "../models/AVLTree.h"

class FriendshipManager{
private:
    AVLTree<std::string> FriendRequest;

public:    
    FriendshipManager();
    ~FriendshipManager();

    bool sendRequest(User& fromUser, User& toUser);
    bool acceptRequest(User& fromUser, User& toUser);
    bool cancelRequest(User& fromUser, User& toUser);

    std::vector<std::string> viewFriends(const User& user);
    std::vector<std::string> suggestedFriends(const User& user, const std::vector<User>& allUsers);  // Fixed!

    bool areFriends(const User& user1, const User& user2);
    int getMutualFriendsCount(const User& user1, const User& user2);
    std::vector<std::string> getMutualFriends(const User& user1, const User& user2);
};

#endif // FRIENDSHIPMANAGER_H (also fixed the endif)
