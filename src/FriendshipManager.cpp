#include "../Headers/FriendshipManager.h"
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
    fromUser.addFriend(toUser.getUsername());
    toUser.addFriend(fromUser.getUsername());
    return true;
}

bool FriendshipManager::cancelRequest(User& fromUser, User& toUser) {
    std::string req = fromUser.getUsername() + "->" + toUser.getUsername();
    auto it = std::find(FriendRequestList.begin(), FriendRequestList.end(), req);
    if (it == FriendRequestList.end()) return false;
    FriendRequestList.erase(it);
    return true;
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
