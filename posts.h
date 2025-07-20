#include <string>
#include <vector>
#include "Comment.h"
#include "crow.h"
using namespace std;

class Posts {
public:
    int postID;
    string author;
    string content;
    string timestamp;
    vector<Comment> comments;

    Posts();
    Posts(int id, const string& a, const string& c, const string& t);

    void addComment(const Comment& c);
    crow::json::wvalue toJSON() const;
    static Posts fromJSON(const crow::json::rvalue& data);
};