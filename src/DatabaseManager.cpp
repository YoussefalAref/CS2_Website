#include "DatabaseManager.h"
#include <sstream> 
#include <exception> 
#include "User.h"
#include <stdexcept>
#include <filesystem>
using namespace std;

// =============== POSTS ===============
vector<Post> file::retrieveData() const {
    vector<Post> posts;
    filesystem::path file_path = filesystem::absolute("../txtFiles/posts.csv");
    ifstream txtFile(file_path);

    if (!txtFile.is_open()) {
        cerr << "Error opening posts file for reading\n";
        return posts;
    }

    string line;
    while (getline(txtFile, line)) {
        if (line.empty()) continue;
        vector<string> fields;
        istringstream lineStream(line);
        string field;
        
        while (getline(lineStream, field, '|')) {
            fields.push_back(field);
        }

        if (fields.size() < 5) {
            cerr << "Invalid post line: " << line << endl;
            continue;
        }
        try {
            posts.emplace_back(
                stoi(fields[0]),  // authorId
                stoi(fields[1]),  // postId
                fields[2],        // author
                fields[3],        // content
                stoi(fields[4])   // likesNo
            );
        } catch (const exception& e) {
            cerr << "Error processing post: " << line << " - " << e.what() << endl;
        }
    }
    txtFile.close();
    return posts;
}

void file::fillData(const vector<Post>& posts) {
    filesystem::path file_path = filesystem::absolute("../txtFiles/posts.csv");
    ofstream txtFile(file_path);
    if (!txtFile.is_open()) {
        cerr << "Error opening posts file for writing\n";
        return;
    }
    for (const Post& po : posts) {
        txtFile << po.getAuthorID() << "|"
                << po.getPostID() << "|"
                << po.getAuthor() << "|"
                << po.getContent() << "|"
                << po.getLikesNo() << "\n";
    }
    txtFile.close();
}

void file::insertData(const Post& post) {
    filesystem::path file_path = filesystem::absolute("../txtFiles/posts.csv");
    ofstream txtFile(file_path, ios::app);
    if (!txtFile.is_open()) {
        cerr << "Error opening posts file for appending\n";
        return;
    }
    txtFile << post.getAuthorID() << "|"
            << post.getPostID() << "|"
            << post.getAuthor() << "|"
            << post.getContent() << "|"
            << post.getLikesNo() << "\n";
    txtFile.close();
}

// =============== USERS ===============
unordered_map<string, User> file::retrieveUsers() const {
    unordered_map<string, User> users;
    filesystem::path file_path = filesystem::absolute("../txtFiles/users.csv");
    ifstream txtFile(file_path);

    if (!txtFile.is_open()) {
        cerr << "Error opening users file for reading\n";
        return users;
    }

    string line;
    while (getline(txtFile, line)) {
        if (line.empty()) continue;
        vector<string> fields;
        istringstream lineStream(line);
        string field;

        while (getline(lineStream, field, '|')) {
            fields.push_back(field);
        }

        if (fields.size() < 3) {
            cerr << "Invalid user line: " << line << endl;
            continue;
        }
        try {
            int userId = stoi(fields[0]);
            string username = fields[1];
            size_t hashedPassword = stoull(fields[2]);
            users.try_emplace(username, userId, username, hashedPassword);
        } catch (const exception& e) {
            cerr << "Error processing user: " << line << " - " << e.what() << endl;
        }
    }
    txtFile.close();
    return users;
}

void file::fillUsers(const unordered_map<string, User>& users) {
    filesystem::path file_path = filesystem::absolute("../txtFiles/users.csv");
    ofstream txtFile(file_path);
    if (!txtFile.is_open()) {
        cerr << "Error opening users file for writing\n";
        return;
    }
    for (const auto& pair : users) {
        const User& user = pair.second;
        txtFile << user.getUserId() << "|"
                << user.getUsername() << "|"
                << user.gethashedPassword() << "\n";
    }
    txtFile.close();
}

void file::insertUser(const User& user) {
    filesystem::path file_path = filesystem::absolute("../txtFiles/users.csv");
    ofstream txtFile(file_path, ios::app);
    if (!txtFile.is_open()) {
        cerr << "Error opening users file for appending\n";
        return;
    }
    txtFile << user.getUserId() << "|"
            << user.getUsername() << "|"
            << user.gethashedPassword() << "\n";
    txtFile.close();
}