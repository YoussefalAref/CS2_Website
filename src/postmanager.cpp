#include "postmanager.h"
#include "DatabaseManager.h"

PostManager::PostManager(int _userId) : userid(_userId) {
    global_posts.clear();
    user_posts.clear();
    global_posts=retrieveData("global");
    for (const auto& post : global_posts) {
        if (post.authorID == userid) {
            user_posts.push_back(post);
        }
    }
}


void PostManager::addPost(int postId,const std::string& author, const std::string& content, const std::string& timestamp) {
    Post newpost(userid, postId, author, content, timestamp);
    global_posts.push_back(newpost);
    user_posts.push_back(newpost);
}

bool PostManager::deletePost(int id) {
    for (auto it = global_posts.begin(); it != global_posts.end(); ++it) {
        if (it->postID == id) {
            global_posts.erase(it);
            return true;
        }
    }
    for (auto it = user_posts.begin(); it != user_posts.end(); ++it) {
        if (it->postID == id) {
            user_posts.erase(it);
            return true;
        }
    }
    return false;
}

bool PostManager::sharePost(const Post& original, const std::string& newAuthor, const std::string& timestamp) {
    Post shared_post(userid, original.postID, newAuthor, original.content, original.timestamp);
    global_posts.push_back(shared_post);
    user_posts.push_back(shared_post);
    return true;
}

// bool PostManager::addComment(int postID, const Comment& comment) {
//     for (auto& post : posts) {
//         if (post.postID == postID) {
//             post.comments.push_back(comment);
//             return true;
//         }
//     }
//     return false;
// }

// Post* PostManager::getPostById(int id) {
//     for (auto& post : posts) {
//         if (post.postID == id)
//             return &post;
//     }
//     return nullptr;
// }

const std::vector<Post>& PostManager::getAllPosts() const {
    return global_posts;
}

// crow::json::wvalue PostManager::toJSON() const {
//     crow::json::wvalue result;
//     int index = 0;
//     for (const auto& post : posts) {
//         result[index]["postID"] = post.postID;
//         result[index]["author"] = post.author;
//         result[index]["content"] = post.content;
//         result[index]["timestamp"] = post.timestamp;
//         int c = 0;
//         for (const auto& comment : post.comments) {
//             result[index]["comments"][c]["text"] = comment.text;
//             result[index]["comments"][c]["author"] = comment.author;
//             result[index]["comments"][c]["timestamp"] = comment.timestamp;
//             c++;
//         }
//         index++;
//     }
//     return result;
// }













// PostManager postManager;  // Global instance

// // Add new post
// CROW_ROUTE(app, "/addpost").methods("POST"_method)([](const crow::request& req) {
//     auto body = crow::json::load(req.body);
//     if (!body) return crow::response(400, "Invalid JSON");

//     std::string author = body["author"].s();
//     std::string content = body["content"].s();
//     std::string timestamp = body["timestamp"].s();

//     postManager.addPost(author, content, timestamp);
//     return crow::response(200, "Post added");
// });

// // Delete post
// CROW_ROUTE(app, "/deletepost").methods("POST"_method)([](const crow::request& req) {
//     auto body = crow::json::load(req.body);
//     if (!body) return crow::response(400, "Invalid JSON");

//     int id = body["postID"].i();
//     bool success = postManager.deletePost(id);

//     return crow::response(success ? 200 : 404, success ? "Deleted" : "Not Found");
// });

// // Add comment
// CROW_ROUTE(app, "/comment").methods("POST"_method)([](const crow::request& req) {
//     auto body = crow::json::load(req.body);
//     if (!body) return crow::response(400, "Invalid JSON");

//     int postID = body["postID"].i();
//     std::string text = body["text"].s();
//     std::string author = body["author"].s();
//     std::string timestamp = body["timestamp"].s();

//     bool success = postManager.addComment(postID, Comment(text, author, timestamp));
//     return crow::response(success ? 200 : 404, success ? "Comment added" : "Post not found");
// });

// // Get all posts
// CROW_ROUTE(app, "/getposts").methods("GET"_method)([]() {
//     return crow::response{postManager.toJSON()};
// });