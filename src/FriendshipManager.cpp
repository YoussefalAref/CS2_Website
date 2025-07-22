#include "FriendshipManager.h"
#include "DatabaseManager.h"
#include <algorithm>

FriendshipManager::FriendshipManager() {}

FriendshipManager::~FriendshipManager() {}

bool FriendshipManager::sendRequest(User& fromUser, User& toUser) {
    if (areFriends(fromUser, toUser)) return false;
    std::string req = fromUser.getUsername() + "->" + toUser.getUsername();
    if (std::find(FriendRequestList.begin(), FriendRequestList.end(), req) != FriendRequestList.end())
        return false; // Already requested
    FriendRequestList.push_back(req);
    return true;
}

bool FriendshipManager::acceptRequest(User& fromUser, User& toUser) {
    std::string req = fromUser.getUsername() + "->" + toUser.getUsername();
    auto it = std::find(FriendRequestList.begin(), FriendRequestList.end(), req);
    if (it == FriendRequestList.end()) return false; // No such request
    
    FriendRequestList.erase(it);
    
    // Add friends to both users
    fromUser.addFriend(toUser.getUsername());
    toUser.addFriend(fromUser.getUsername());
    
    // Update database with new friend lists
    file dbManager;
    dbManager.updateUser(fromUser);
    dbManager.updateUser(toUser);
    
    return true;
}

bool FriendshipManager::cancelRequest(User& fromUser, User& toUser) {
    std::string req = fromUser.getUsername() + "->" + toUser.getUsername();
    auto it = std::find(FriendRequestList.begin(), FriendRequestList.end(), req);
    if (it == FriendRequestList.end()) return false;
    FriendRequestList.erase(it);
    return true;
}

bool FriendshipManager::removeFriendship(User& user1, User& user2) {
    if (!areFriends(user1, user2)) return false;
    
    // Remove from both users
    user1.removeFriend(user2.getUsername());
    user2.removeFriend(user1.getUsername());
    
    // Update database
    file dbManager;
    dbManager.updateUser(user1);
    dbManager.updateUser(user2);
    
    return true;
}

std::vector<std::string> FriendshipManager::viewFriends(const User& user) {
    return user.getFriendsList();
}

std::vector<std::string> FriendshipManager::suggestedFriends(const User& user, const std::vector<User>& allUsers) {
    std::vector<std::string> suggestions;
    std::vector<std::string> userFriends = user.getFriendsList();
    
    for (const auto& potentialFriend : allUsers) {
        // Skip if it's the same user
        if (potentialFriend.getUsername() == user.getUsername()) continue;
        
        // Skip if already friends
        if (user.isFriend(potentialFriend.getUsername())) continue;
        
        // Check for mutual friends
        int mutualCount = 0;
        std::vector<std::string> potentialFriendsList = potentialFriend.getFriendsList();
        
        for (const auto& userFriend : userFriends) {
            if (std::find(potentialFriendsList.begin(), potentialFriendsList.end(), userFriend) != potentialFriendsList.end()) {
                mutualCount++;
            }
        }
        
        // Suggest if they have mutual friends
        if (mutualCount > 0) {
            suggestions.push_back(potentialFriend.getUsername());
        }
    }
    
    // Sort by mutual friends count (you might want to store count with suggestions for better sorting)
    return suggestions;
}

bool FriendshipManager::areFriends(const User& user1, const User& user2) {
    return user1.isFriend(user2.getUsername()) && user2.isFriend(user1.getUsername());
}

int FriendshipManager::getMutualFriendsCount(const User& user1, const User& user2) {
    return getMutualFriends(user1, user2).size();
}

std::vector<std::string> FriendshipManager::getMutualFriends(const User& user1, const User& user2) {
    std::vector<std::string> mutual;
    auto friends1 = user1.getFriendsList();
    auto friends2 = user2.getFriendsList();
    
    for (const auto& f : friends1) {
        if (std::find(friends2.begin(), friends2.end(), f) != friends2.end()) {
            mutual.push_back(f);
        }
    }
    return mutual;
}