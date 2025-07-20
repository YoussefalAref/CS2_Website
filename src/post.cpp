#include "post.h"
#include "crow.h"
Post::Post(){}

Post::Post(int _authorId,int id, const string& a, const string& c, const string& t): authorID(_authorId), postID(id), author(a), content(c), timestamp(t) {}

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







// CROW_ROUTE(app, "/sharepost").methods("POST"_method)([](const crow::request& req) {
//     auto body = crow::json::load(req.body);
//     if (!body) return crow::response(400, "Invalid JSON");

//     Post original = Post::fromJSON(body["originalPost"]);
//     std::string newAuthor = body["newAuthor"].s();
//     std::string timestamp = body["timestamp"].s();

//     postManager.sharePost(original, newAuthor, timestamp);
//     return crow::response(200, "Post shared");
// });
