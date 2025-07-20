#include <vector>
#include <string>
#include "crow.h"
#include "Post.h"
#include "Comment.h"

class PostManager {
private:
    std::vector<Post> posts;
    int nextPostID = 1;

public:
    void addPost(const std::string& author, const std::string& content, const std::string& timestamp);
    bool deletePost(int id);
    bool sharePost(const Post& original, const std::string& newAuthor, const std::string& timestamp);
    bool addComment(int postID, const Comment& comment);
    const std::vector<Post>& getAllPosts() const;
    Post* getPostById(int id);
    crow::json::wvalue toJSON() const; 
};