#include "FriendshipManager.h"
#include <algorithm>

FriendshipManager::FriendshipManager() {}

FriendshipManager::~FriendshipManager() {}

bool FriendshipManager::sendRequest(User& fromUser, User& toUser) {
    // Check if they're already friends
    if (areFriends(fromUser, toUser)) {
        return false;
    }
    
    // Check if request already sent
    auto sentRequests = fromUser.getSentRequests();
    if (std::find(sentRequests.begin(), sentRequests.end(), toUser.getUsername()) != sentRequests.end()) {
        return false;
    }
    
    // Check if there's already a pending request from the other user
    auto pendingRequests = toUser.getPendingRequests();
    if (std::find(pendingRequests.begin(), pendingRequests.end(), fromUser.getUsername()) != pendingRequests.end()) {
        return false;
    }
    
    // Send the request - update both users
    bool senderSuccess = fromUser.sendFriendRequest(toUser.getUsername());
    if (senderSuccess) {
        toUser.addPendingRequest(fromUser.getUsername());
    }
    
    return senderSuccess;
}

bool FriendshipManager::acceptRequest(User& fromUser, User& toUser) {
    // Check if there's a pending request from fromUser to toUser
    auto pendingRequests = toUser.getPendingRequests();
    auto it = std::find(pendingRequests.begin(), pendingRequests.end(), fromUser.getUsername());
    
    if (it == pendingRequests.end()) {
        return false; // No pending request found
    }
    
    // Accept the request
    bool success = toUser.acceptFriendRequest(fromUser.getUsername());
    
    if (success) {
        // Add the reverse friendship
        fromUser.addFriend(toUser.getUsername());
        
        // Clean up sender's sent requests
        fromUser.cancelSentRequest(toUser.getUsername());
    }
    
    return success;
}

bool FriendshipManager::cancelRequest(User& fromUser, User& toUser) {
    // Check if request was sent from fromUser to toUser
    auto sentRequests = fromUser.getSentRequests();
    auto it = std::find(sentRequests.begin(), sentRequests.end(), toUser.getUsername());
    
    if (it == sentRequests.end()) {
        return false; // No sent request found
    }
    
    // Cancel the request from both sides
    bool senderSuccess = fromUser.cancelSentRequest(toUser.getUsername());
    if (senderSuccess) {
        toUser.removePendingRequest(fromUser.getUsername());
    }
    
    return senderSuccess;
}

std::vector<std::string> FriendshipManager::viewFriends(const User& user) {
    return user.getFriendsList();
}

std::vector<std::string> FriendshipManager::suggestedFriends(const User& user, const std::vector<User>& allUsers) {
    return user.suggestFriends(allUsers);
}

bool FriendshipManager::areFriends(const User& user1, const User& user2) {
    return user1.isFriend(user2.getUsername()) && user2.isFriend(user1.getUsername());
}

int FriendshipManager::getMutualFriendsCount(const User& user1, const User& user2) {
    auto mutualFriends = getMutualFriends(user1, user2);
    return mutualFriends.size();
}

std::vector<std::string> FriendshipManager::getMutualFriends(const User& user1, const User& user2) {
    return user1.getMutualFriends(user2);
}
