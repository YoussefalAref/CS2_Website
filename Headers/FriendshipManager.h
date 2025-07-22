#ifndef FRIENDSHIPMANAGER_H
#define FRIENDSHIPMANAGER_H

#include <vector>
#include <string>

class User;

class FriendshipManager {
private:
    std::vector<std::string> FriendRequestList;

public:
    FriendshipManager();
    ~FriendshipManager();
    
    // Friend request management
    bool sendRequest(User& fromUser, User& toUser);
    bool acceptRequest(User& fromUser, User& toUser);
    bool cancelRequest(User& fromUser, User& toUser);
    
    // Friend management
    bool removeFriendship(User& user1, User& user2);
    std::vector<std::string> viewFriends(const User& user);
    std::vector<std::string> suggestedFriends(const User& user, const std::vector<User>& allUsers);
    
    // Friend status checking
    bool areFriends(const User& user1, const User& user2);
    int getMutualFriendsCount(const User& user1, const User& user2);
    std::vector<std::string> getMutualFriends(const User& user1, const User& user2);
};

#endif