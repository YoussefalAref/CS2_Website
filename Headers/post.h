#pragma once
#include <string>
#include <vector>
#include "crow.h"
#include "comment.h"
using namespace std;

class Post{
private:
    int likesNO;
    int postID;
    string author;
    string content;
    int authorID;
    vector<Comment> comments;

public:
    Post();
    Post(int _authorId, int id, const string& a, const string& c, int _likesNo = 0);

    void setPostID(int id);
    void setAuthor(string username);
    void setContent(string cont);
    void setAuthorID(int id);
    void setLikesNo(int likes);

    int getPostID()const;
    string getAuthor()const;
    string getContent()const;
    int getAuthorID()const;
    int getLikesNo()const;

    

    bool deleteComment(int commentId);
    void addComment(const Comment& c);
    // crow::json::wvalue toJSON() const;
    // static Post fromJSON(const crow::json::rvalue& data);
};