#include <string>
#include <vector>

#include "crow.h"
using namespace std;

class Post {
public:
    int postID;
    string author;
    string content;
    string timestamp;
    // vector<Comment> comments;

    Post();
    Post(int id, const string& a, const string& c, const string& t);

    // void addComment(const Comment& c);
    crow::json::wvalue toJSON() const;
    static Post fromJSON(const crow::json::rvalue& data);
};