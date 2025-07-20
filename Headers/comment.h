#include <string>
#include "crow.h"
using namespace std;

class Comment {
public:
    int commentID;
    string content;
    int publisherID;
    string username;
    int postID;


    Comment();
    Comment(const int& comId,const string& c,const int& pubId, const string& u, const int& postId);

    crow::json::wvalue toJSON() const;
    static Comment fromJSON(const crow::json::rvalue& data);
};