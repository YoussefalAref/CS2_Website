#pragma once
#include <string>
#include <vector>
#include "crow.h"
using namespace std;

class Post{
private:
    int postID;
    string author;
    string content;
    int authorID;
    // vector<Comment> comments;

public:

    string timestamp;

    Post();
    Post(int _authorId,int id, const string& a, const string& c,const string& t="");

    // void setPostID(int id);
    // void setAuthor(string username);
    // void setContent(string cont);
    // void setAuthorID(int id);

    int getPostID()const;
    string getAuthor()const;
    string getContent()const;
    int getAuthorID()const;

    

    
    // void addComment(const Comment& c);
    // crow::json::wvalue toJSON() const;
    // static Post fromJSON(const crow::json::rvalue& data);
};