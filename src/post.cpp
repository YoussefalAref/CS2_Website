#include <chrono>
#include <iomanip>
#include <sstream>
#include "post.h"
#include "crow.h"
using namespace std;


Post::Post(){}
Post::Post(int _authorId,int id, const string& a, const string& c, const string& t)
:authorID(_authorId), postID(id), author(a), content(c){
    if (t.empty()) {
        // Auto-generate timestamp if none provided
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        stringstream ss;
        ss << put_time(localtime(&now_time), "%Y-%m-%d %H:%M:%S");
        timestamp = ss.str();
    } else {
        timestamp = t;
    }
}

int Post::getPostID()const{return postID;}   
string Post::getAuthor()const{return author;}
string Post::getContent()const{return content;}
int Post::getAuthorID()const{return authorID;}

// void Post::addComment(const Comment& c) {
//     comments.push_back(c);
// }

// crow::json::wvalue Post::toJSON() const {
//     crow::json::wvalue result;
//     result["postID"] = postID;
//     result["author"] = author;
//     result["content"] = content;
//     result["timestamp"] = timestamp;

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
//         data["timestamp"].s()
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
// //     std::string timestamp = body["timestamp"].s();

// //     postManager.sharePost(original, newAuthor, timestamp);
// //     return crow::response(200, "Post shared");
// // });
