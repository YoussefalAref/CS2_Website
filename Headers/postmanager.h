#include <vector>
#include <string>
#include "post.h"
#include "crow.h"


class PostManager {
private:
    std::vector<Post> global_posts;
    std::vector<Post> user_posts;
    int userid;

public:
    PostManager(int _userId);
    ~PostManager();
    void addPost(int postId,const std::string& author, const std::string& content, const std::string& timestamp);
    bool deletePost(int id);
    bool sharePost(const Post& original, const std::string& newAuthor, const std::string& timestamp);
    // bool addComment(int postID, const Comment& comment);
    const std::vector<Post>& getAllPosts() const;
    Post* getPostById(int id);
    crow::json::wvalue toJSON() const; 
};