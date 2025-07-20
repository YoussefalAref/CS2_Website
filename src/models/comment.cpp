#include "Comment.h"

Comment::Comment() {}

Comment::Comment(const string& t, const string& a, const string& ts)
    : text(t), author(a), timestamp(ts) {}

crow::json::wvalue Comment::toJSON() const {
    crow::json::wvalue result;
    result["text"] = text;
    result["author"] = author;
    result["timestamp"] = timestamp;
    return result;
}

Comment Comment::fromJSON(const crow::json::rvalue& data) {
    return Comment(
        data["text"].s(),
        data["author"].s(),
        data["timestamp"].s()
    );
}