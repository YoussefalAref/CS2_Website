/*to do
pfp logic
post logic
*/

#include "User.h"
#include "post.h"
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <crow/json.h>

User::User() 
    : username(""), email(""), hashedPassword(0)
    , lastActive(std::chrono::system_clock::now()), 
    isActive(true) {}

    User::User(const std::string& uname, const std::string& password)
    : username(uname), hashedPassword(hashPassword(password)) {}

User::User(const std::string& uname, const std::string& password)
    : username(uname), email(""), hashedPassword(hashPassword(password)),
    lastActive(std::chrono::system_clock::now()),
    isActive(true) {}

User::User(const std::string& uname, const std::string& password, const std::string& userEmail)
    : username(uname), email(userEmail), hashedPassword(hashPassword(password)),
    lastActive(std::chrono::system_clock::now()),
    isActive(true) {}

User::~User() {}

size_t User::hashPassword(const std::string& password) {
    std::hash<std::string> hasher;
    return hasher(password);
}

bool User::checkPassword(const std::string& password) const {
    return hashedPassword == hashPassword(password);
}


void User::changePassword(const std::string& oldPassword, const std::string& newPassword){
    if (checkPassword(oldPassword))
        hashedPassword = hashPassword(newPassword);
}

void User::setEmail(const std::string& userEmail) {
    email = userEmail;
}


void User::setLastActive() {
    lastActive = std::chrono::system_clock::now();
}

void User::setActive(bool active) {
    isActive = active;
}

// Getters
std::string User::getUsername() const { return username; }
std::string User::getEmail() const { return email; }
std::chrono::system_clock::time_point User::getLastActive() const { return lastActive; }
bool User::getIsActive() const { return isActive; }
size_t User::getHashedPassword() const { return hashedPassword; }

// Friend management
bool User::addFriend(const std::string& friendUsername) {
    if (friendUsername == username) return false; // Can't friend yourself
    if (isFriend(friendUsername)) return false;   // Already friends
    
    friends.insert(friendUsername);
    return true;
}

bool User::removeFriend(const std::string& friendUsername) {
    if (!isFriend(friendUsername)) return false;
    
    friends.remove(friendUsername);
    return true;
}

bool User::isFriend(const std::string& username) const {
    return friends.search(username);
}

std::vector<std::string> User::getFriendsList() const {
    return friends.getAllElements();
}

int User::getFriendsCount() const {
    return friends.size();
}

// Friend request management
bool User::sendFriendRequest(const std::string& toUsername) {
    if (toUsername == username) return false; // Can't send to yourself
    
    // Check if already sent
    auto it = std::find(sentRequests.begin(), sentRequests.end(), toUsername);
    if (it != sentRequests.end()) return false;
    
    // Check if already friends
    if (isFriend(toUsername)) return false;
    
    sentRequests.push_back(toUsername);
    return true;
}

bool User::acceptFriendRequest(const std::string& fromUsername) {
    auto it = std::find(pendingRequests.begin(), pendingRequests.end(), fromUsername);
    if (it == pendingRequests.end()) return false;
    
    friends.insert(fromUsername);
    
    pendingRequests.erase(it);
    return true;
}

bool User::rejectFriendRequest(const std::string& fromUsername) {
    auto it = std::find(pendingRequests.begin(), pendingRequests.end(), fromUsername);
    if (it == pendingRequests.end()) return false;
    
    pendingRequests.erase(it);
    return true;
}

bool User::cancelSentRequest(const std::string& toUsername) {
    auto it = std::find(sentRequests.begin(), sentRequests.end(), toUsername);
    if (it == sentRequests.end()) return false;
    
    sentRequests.erase(it);
    return true;
}

void User::addPendingRequest(const std::string& fromUsername) {
    pendingRequests.push_back(fromUsername);
}

std::vector<std::string> User::getPendingRequests() const {
    return pendingRequests;
}

std::vector<std::string> User::getSentRequests() const {
    return sentRequests;
}

void User::removePendingRequest(const std::string& fromUsername) {
    auto it = std::find(pendingRequests.begin(), pendingRequests.end(), fromUsername);
    if (it != pendingRequests.end()) {
        pendingRequests.erase(it);
    }
}

//friend suggestions and mutial friends
std::vector<std::string> User::getMutualFriends(const User& otherUser) const{
    std::vector<std::string> mutualFriends;
    std::vector<std::string> myFriends = getFriendsList();
    std::vector<std::string> theirFriends = otherUser.getFriendsList();

    //find intersection in friends list
    for(const auto& myFriend : myFriends){
        for(const auto& theirFriend : theirFriends){
            if (myFriend == theirFriend){
                mutualFriends.push_back(myFriend);
                break;
            }
        }
    }
    return mutualFriends;
}

std::vector<std::string> User::suggestFriends(const std::vector<User>& allUsers) const {
    std::vector<std::string> suggestions;
    std::vector<std::string> myFriends = getFriendsList();
    
    for (const auto& user : allUsers) {
        // Skip if it's the same user
        if (user.getUsername() == username) continue;
        
        // Skip if already friends
        if (isFriend(user.getUsername())) continue;
        
        // Skip if already sent a request
        auto sentIt = std::find(sentRequests.begin(), sentRequests.end(), user.getUsername());
        if (sentIt != sentRequests.end()) continue;
        
        // Skip if there's a pending request
        auto pendingIt = std::find(pendingRequests.begin(), pendingRequests.end(), user.getUsername());
        if (pendingIt != pendingRequests.end()) continue;
        
        // Calculate mutual friends count
        std::vector<std::string> mutualFriends = getMutualFriends(user);
        int mutualCount = mutualFriends.size();
        
        // Suggest if there are mutual friends (you can adjust this threshold)
        if (mutualCount > 0) {
            suggestions.push_back(user.getUsername());
        }
    }
    
    // Sort by mutual friends count (descending)
    std::sort(suggestions.begin(), suggestions.end(), 
        [this, &allUsers](const std::string& a, const std::string& b) {
            auto userA = std::find_if(allUsers.begin(), allUsers.end(), 
                [&a](const User& u) { return u.getUsername() == a; });
            auto userB = std::find_if(allUsers.begin(), allUsers.end(), 
                [&b](const User& u) { return u.getUsername() == b; });
            
            if (userA == allUsers.end() || userB == allUsers.end()) return false;
            
            int mutualA = getMutualFriends(*userA).size();
            int mutualB = getMutualFriends(*userB).size();
            
            return mutualA > mutualB;
        });
    
    // Limit suggestions to top 10
    if (suggestions.size() > 10) {
        suggestions.resize(10);
    }
    
    return suggestions;
}

//Post management
bool comparePostsByTime(const std::shared_ptr<Post>& a, const std::shared_ptr<Post>& b) {
    return a->getTimestamp() > b->getTimestamp();
}//standalone function

void User::addPost(std::shared_ptr<Post> post){
    userPosts.push_back(post);
    
    // Sort using simple function (newest first)
    std::sort(userPosts.begin(), userPosts.end(), comparePostsByTime);
}

bool User::removePost(const std::string& postId){
    for (auto i = userPosts.begin(); i != userPosts.end(); ++i){
        if((*i)->getPostId() == postId){
            userPosts.erase(i);
            return true;
        }
    }
    return false;
}

std::vector<std::shared_ptr<Post>> User::getUserPosts() const {
    return userPosts;
}

std::vector<std::shared_ptr<Post>> User::getRecentPosts(int limit) const {
    std::vector<std::shared_ptr<Post>> recentPosts;
    
    int count = std::min(limit, static_cast<int>(userPosts.size()));
    for (int i = 0; i < count; ++i) {
        recentPosts.push_back(userPosts[i]);
    }
    
    return recentPosts;
}

// validation
bool User::isValidUser(const std::string& username) const{
    return !username.empty() && username.length() >= 3 && username.length() <= 20;
}

bool User::isValidEmail(const std::string& email) const {
    return email.find('@') != std::string::npos && email.length() >= 5;
}

// Operators
bool User::operator==(const User& other) const {
    return username == other.username;
}

bool User::operator!=(const User& other) const {
    return !(*this == other);
}

//to and from json
std::string User::toJson() const{
    crow::json::wvalue json;

    //user info
    json["username"] = username;
    json["email"] = email;
    json["hashedPassword"] = std::to_string(hashedPassword);
    json["isActive"] = isActive;

    auto time_t = std::chrono::system_clock::to_time_t(lastActive);//converting time to string
    json["lastActive"] = std::to_string(time_t);

    auto friendsList = friends.inorderTraversal();
    for (size_t i = 0; i < friendsList.size(); ++i) {
        json["friends"][i] = friendsList[i];
    }

    for (size_t i = 0; i < pendingRequests.size(); ++i) {
        json["pendingRequests"][i] = pendingRequests[i];
    }
    
    for (size_t i = 0; i < sentRequests.size(); ++i) {
        json["sentRequests"][i] = sentRequests[i];
    }

    return json.dump();
}

User User::fromJson(const std::string& jsonStr) {
    auto json = crow::json::load(jsonStr);
    
    User user;
    user.username = json["username"].s();
    user.email = json["email"].s();
    user.hashedPassword = std::stoull(json["hashedPassword"].s());
    user.isActive = json["isActive"].b();
    
    auto time_t = std::stoll(json["lastActive"].s());
    user.lastActive = std::chrono::system_clock::from_time_t(time_t);
    
    if (json.has("friends")) {
        auto friendsArray = json["friends"];
        for (const auto& friendName : friendsArray) {
            user.friends.insert(friendName.s());
        }
    }
    
    if (json.has("pendingRequests")) {
        auto requests = json["pendingRequests"];
        for (const auto& request : requests) {
            user.pendingRequests.push_back(request.s());
        }
    }
    
    if (json.has("sentRequests")) {
        auto requests = json["sentRequests"];
        for (const auto& request : requests) {
            user.sentRequests.push_back(request.s());
        }
    }
    
    return user;
}
