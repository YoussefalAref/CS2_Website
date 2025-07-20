#include "Comment.h"

Comment::Comment() {}

Comment::Comment(const int& comId,const string& c,const int& pubId, const string& u, const int& postId)
    : commentID(comId),content(c),publisherID(pubId),username(u),postID(postId){}

/*crow::json::wvalue Comment::toJSON() const {
    crow::json::wvalue result;
    result["text"] = content;
    result["author"] = username;
    result["comment ID"]= commentID;
    result["user ID"]= publisherID;
    result["post ID"]= postID;
    return result;
}

Comment Comment::fromJSON(const crow::json::rvalue& data) {
    return Comment(
        data["text"].s(),
        data["author"].s(),
        data["comment ID"].s(),
        data["user ID"].s(),
        data["post ID"].s(),
    );
}*/
