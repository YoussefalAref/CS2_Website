#include "comment.h"


Comment::Comment() {}

Comment::Comment(int comId, const std::string& c, int authId, int postId)
    : commentId(comId), content(c), authorId(authId), parentPostId(postId) {}

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
