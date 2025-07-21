#include <chrono>
#include <iomanip>
#include <sstream>
#include "post.h"
#include "crow.h"
using namespace std;


Post::Post(){}
Post::Post(int _authorId,int _postid, const string& a, const string& c,int _likesNo)
:authorID(_authorId), postID(_postid), author(a), content(c),likesNO(_likesNo){}

int Post::getPostID()const{return postID;}   
string Post::getAuthor()const{return author;}
string Post::getContent()const{return content;}
int Post::getAuthorID()const{return authorID;}
int Post::getLikesNo()const { return likesNO; }

void Post::setPostID(int id) { postID = id; }
void Post::setAuthor(string username) { author = username; }
void Post::setContent(string cont) { content = cont; }
void Post::setAuthorID(int id) { authorID = id; }
void Post::setLikesNo(int likes) { likesNO = likes; }


void Post::addComment(const Comment& c) {
    comments.push_back(c);
}

bool Post::deleteComment(int commentId) {
    auto it = std::remove_if(comments.begin(), comments.end(), [&](const Comment& c) {
        return c.commentId == commentId;
    });
    if (it != comments.end()) {
        comments.erase(it, comments.end());
        return true;
    }
    return false;
}

// crow::json::wvalue Post::toJSON() const {
//     crow::json::wvalue result;
//     result["postID"] = postID;
//     result["author"] = author;
//     result["content"] = content;
//     // result["timestamp"] = timestamp; // removed: no timestamp in Comment

//     // int index = 0;
//     // for (const auto& comment : comments) {
//     //     result["comments"][index++] = comment.toJSON();
//     // }

//     return result;
// }

// Post Post::fromJSON(const crow::json::rvalue& data) {
//     Post post(
//         data["postID"].i(),
//         data["author"].s(),
//         data["content"].s(),
//         // data["timestamp"].s() // removed: no timestamp in Comment
//     );

//     if (data.has("comments")) {
//         for (size_t i = 0; i < data["comments"].size(); ++i) {
//             post.addComment(Comment::fromJSON(data["comments"][i]));
//         }
//     }

//     return post;
// }







// // CROW_ROUTE(app, "/sharepost").methods("POST"_method)([](const crow::request& req) {
// //     auto body = crow::json::load(req.body);
// //     if (!body) return crow::response(400, "Invalid JSON");

// //     Post original = Post::fromJSON(body["originalPost"]);
// //     std::string newAuthor = body["newAuthor"].s();
// //     // std::string timestamp = body["timestamp"].s(); // removed: no timestamp in Comment

// //     postManager.sharePost(original, newAuthor); // removed: no timestamp in Comment
// //     return crow::response(200, "Post shared");
// // });
