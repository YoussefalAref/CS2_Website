#include "User.h"
#include <functional>
#include <algorithm>
#include <sstream>
#include "postmanager.h"
#include "FriendshipManager.h"
using namespace std;

User::User(int _userId, const std::string& uname, const std::string& password)
    : userId(_userId), username(uname), hashedPassword(hashPassword(password)), 
      postmanager(new PostManager(_userId)), friendshipmanager(new FriendshipManager()) {}

User::User(int _userId, const std::string& uname, size_t passwordHash)
    : userId(_userId), username(uname), hashedPassword(passwordHash), 
      postmanager(new PostManager(_userId)), friendshipmanager(new FriendshipManager()) {}

User::~User() {
    delete postmanager;
    delete friendshipmanager;
}
User::User(){
    userId = -1; // Default invalid ID
    username = "";
    hashedPassword = 0;
    postmanager = nullptr;
    friendshipmanager = nullptr;
}

size_t User::hashPassword(const std::string& password) {
    std::hash<std::string> hasher;
    return hasher(password);
}

std::string User::getUsername() const {
    return username;
}

size_t User::gethashedPassword() const {
    return hashedPassword;
}

int User::getUserId() const {
    return userId;
}

bool User::checkPassword(const std::string& password) const {
    return hashedPassword == hashPassword(password);
}

// Friend management
bool User::addFriend(const std::string& friendUsername) {
    if (friendUsername == username) return false; // Can't friend yourself
    if (friendsSet.find(friendUsername) != friendsSet.end()) return false; // Already friends
    
    friendsSet.insert(friendUsername);
    friendsAVL.addFriend(friendUsername);
    return true;
}

bool User::removeFriend(const std::string& friendUsername) {
    if (friendsSet.find(friendUsername) == friendsSet.end()) return false;
    
    friendsSet.erase(friendUsername);
    friendsAVL.removeFriend(friendsAVL.root, friendUsername);
    return true;
}

bool User::isFriend(const std::string& friendUsername) const {
    return friendsSet.find(friendUsername) != friendsSet.end();
}

std::vector<std::string> User::getFriendsList() const {
    std::vector<std::string> friendsList(friendsSet.begin(), friendsSet.end());
    std::sort(friendsList.begin(), friendsList.end()); // Return sorted list
    return friendsList;
}

int User::getFriendsCount() const {
    return friendsSet.size();
}

void User::setFriendsList(const std::vector<std::string>& friends) {
    friendsSet.clear();
    // Clear AVL tree (you'll need to add a clear method to AVLTree)
    // friendsAVL.clear();
    
    for (const auto& friendName : friends) {
        friendsSet.insert(friendName);
        friendsAVL.addFriend(friendName);
    }
}

// Convert friends list to pipe-separated string for database storage
std::string User::getFriendsAsString() const {
    if (friendsSet.empty()) return "";
    
    std::stringstream ss;
    bool first = true;
    for (const auto& friendName : friendsSet) {
        if (!first) ss << ",";
        ss << friendName;
        first = false;
    }
    return ss.str();
}

// Load friends from pipe-separated string from database
void User::loadFriendsFromString(const std::string& friendsStr) {
    if (friendsStr.empty()) return;
    
    friendsSet.clear();
    // Clear AVL tree
    
    std::stringstream ss(friendsStr);
    std::string friendName;
    
    while (std::getline(ss, friendName, ',')) {
        if (!friendName.empty()) {
            friendsSet.insert(friendName);
            friendsAVL.addFriend(friendName);
        }
    }
}