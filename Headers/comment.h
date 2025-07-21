#include <string>
#include "crow.h"
using namespace std;

class Comment {
public:
    int commentId;          // Unique identifier for the comment
    std::string content;    // The actual comment text
    int authorId;           // Reference to user who posted it
    int parentPostId;       // Reference to the post

    Comment();
    Comment(int comId, const std::string& c, int authId, int postId);

    crow::json::wvalue toJSON() const;
    static Comment fromJSON(const crow::json::rvalue& data);
};