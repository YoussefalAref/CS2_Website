#define CROW_USE_ASIO
#include "crow.h"
#include "./Headers/AuthManager.h"
#include <unordered_map>
#include <mutex>
#include <string>
#include <random>
#include <sstream>
#include <memory>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Enhanced CORS Middleware
struct CORSMiddleware {
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context& /*ctx*/) {
        // Get request origin
        std::string origin = req.get_header_value("Origin");
        if (origin.empty()) origin = "*";

        // Set CORS headers
        res.set_header("Access-Control-Allow-Origin", origin);
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        res.set_header("Access-Control-Allow-Credentials", "true");

    }

    void after_handle(crow::request& /*req*/, crow::response& /*res*/, context& /*ctx*/) {}
};
string getMimeType(const string& filename) {
    if (filename.find(".html") != string::npos) return "text/html";
    if (filename.find(".css") != string::npos) return "text/css";
    if (filename.find(".js") != string::npos) return "application/javascript";
    if (filename.find(".png") != string::npos) return "image/png";
    if (filename.find(".jpg") != string::npos || filename.find(".jpeg") != string::npos) return "image/jpeg";
    if (filename.find(".gif") != string::npos) return "image/gif";
    if (filename.find(".svg") != string::npos) return "image/svg+xml";
    if (filename.find(".woff") != string::npos) return "font/woff";
    if (filename.find(".woff2") != string::npos) return "font/woff2";
    if (filename.find(".ttf") != string::npos) return "font/ttf";
    if (filename.find(".ico") != string::npos) return "image/x-icon";
    return "text/plain";
}

int main()
{
    crow::App<CORSMiddleware> app;

    auto auth = std::make_shared<AuthManager>();


    // Serve HTML file for root endpoint
    CROW_ROUTE(app, "/")([] {
        try {
            std::string abs_path = "../static/website_home.html";
            cout<<abs_path<<endl;
            std::cout << "Current path: " << fs::current_path() << std::endl;
            ifstream file(abs_path);
            if (!file) {
                return crow::response(404, "website_home.html not found");
            }
            
            string content((istreambuf_iterator<char>(file)), 
                       istreambuf_iterator<char>());
            
            crow::response res(content);
            res.set_header("Content-Type", "text/html");
            
            // Relax CSP for development
            res.set_header("Content-Security-Policy", "default-src * 'unsafe-inline' 'unsafe-eval'; style-src * 'unsafe-inline'; img-src * data:; font-src *;");
            
            return res;
        } catch (const exception& e) {
            return crow::response(500, string("Error loading HTML file: ") + e.what());
        }
    });

    // Signup endpoint with JSON error responses
    CROW_ROUTE(app, "/api/signup").methods("POST"_method)
    ([auth](const crow::request& req) {
        crow::response res;
        res.set_header("Content-Type", "application/json");
        
    
        auto body = crow::json::load(req.body);
        crow::json::wvalue result;
        
        if (!body) {
            res.code = 400;
            result["success"] = false;
            result["message"] = "Invalid JSON";
            res.write(result.dump());
            return res;
        }

        if (!body.has("username") || !body.has("password")) {
            res.code = 400;
            result["success"] = false;
            result["message"] = "Missing username or password";
            res.write(result.dump());
            return res;
        }

        try {
            std::string username = body["username"].s();
            std::string password = body["password"].s();
            bool success = auth->registerUser(username,password);

            if (success) {
                result["success"] = true;
                result["username"] = username;
                result["avatar"] = "https://via.placeholder.com/150/1DB954/FFFFFF?text=" + username.substr(0, 1);
                res.code = 200;
            } else {
                result["success"] = false;
                result["message"] = "Username already exists";
                res.code = 409;
            }
        } catch (const std::exception& e) {
            res.code = 500;
            result["success"] = false;
            result["message"] = "Internal server error";
        }

        
        res.write(result.dump());
        return res;
    });


//login endpoint
    CROW_ROUTE(app, "/api/login").methods("POST"_method)
    ([auth](const crow::request& req) {
        crow::response res;
        res.set_header("Content-Type", "application/json");

        auto body = crow::json::load(req.body);
        crow::json::wvalue result;

        if (!body) {
            res.code = 400;
            result["success"] = false;
            result["message"] = "Bad Request";
            res.write(result.dump());
            return res;
        }

        try {
            std::string username = body["username"].s();
            std::string password = body["password"].s();            
            string token = auth->loginUser(username, password);

            if (!token.empty()) {
                result["success"] = true;
                result["username"] = username;
                result["avatar"] = "https://via.placeholder.com/150/1DB954/FFFFFF?text=" + username.substr(0, 1);
                result["token"] = token;
                res.code = 200;
                
                // Set cookie without Secure flag (since using HTTP)
                res.set_header("Set-Cookie", 
                    "token=" + token + 
                    "; Path=/" + 
                    "; SameSite=Strict" + 
                    "; HttpOnly"  // No Secure flag for HTTP
                );
            } else {
                result["success"] = false;
                result["message"] = "Invalid username or password";
                res.code = 401;
            }
        } catch (const std::exception& e) {
            res.code = 500;
            result["success"] = false;
            result["message"] = "Internal server error";
        }

        res.write(result.dump());
        return res;
    });


    // Home endpoint
    CROW_ROUTE(app, "/Home").methods("GET"_method)
    ([auth](const crow::request& req) {
        crow::response res;

        try {
            string token = auth->token_authentication(req);
            if (token.empty()) {
                res.code = 401;
                res.set_header("WWW-Authenticate", "Bearer");
                res.write("Unauthorized: Missing Authorization header");
                return res;
            }

            string* username = auth->getUsernameFromToken(token);
            if (username != nullptr) {
                // Read the Home.html file
                std::ifstream file("../static/Home.html"); // Adjust path as needed
                if (file) {
                    std::string content((std::istreambuf_iterator<char>(file)), 
                                    std::istreambuf_iterator<char>());
                    res.code = 200;
                    res.set_header("Content-Type", "text/html");
                    res.write(content);
                } else {
                    res.code = 404;
                    res.write("Home file not found");
                }
            } else {
                res.code = 401;
                res.set_header("WWW-Authenticate", "Bearer");
                res.write("Unauthorized: Invalid token");
            }
        } catch (const std::exception& e) {
            res.code = 500;
            res.write("Internal server error");
        }

        return res;
    });

   
//     // Friend Request API
// CROW_ROUTE(app, "/api/friend_request").methods("POST"_method)
// ([auth](const crow::request& req) {
//     crow::response res;
//     res.set_header("Content-Type", "application/json"); 
//     crow::json::wvalue result;

//     // Authenticate user from token
//     string token = auth->token_authentication(req);
//     if (token.empty()) {
//         res.code = 401;
//         result["success"] = false;
//         result["message"] = "Unauthorized: Invalid or missing token";
//         res.write(result.dump());
//         return res;
//     }

//     // Get the username from token
//     string* requesterUsernamePtr = auth->getUsernameFromToken(token);
//     if (!requesterUsernamePtr) {
//         res.code = 401;
//         result["success"] = false;
//         result["message"] = "Unauthorized: Invalid token";
//         res.write(result.dump());
//         return res;
//     }
//     string requesterUsername = *requesterUsernamePtr;

//     // Parse request body
//     auto body = crow::json::load(req.body);
//     if (!body) {
//         res.code = 400;
//         result["success"] = false;
//         result["message"] = "Invalid JSON";
//         res.write(result.dump());
//         return res;
//     }

//     if (!body.has("friend_username")) {
//         res.code = 400;
//         result["success"] = false;
//         result["message"] = "Missing friend_username";
//         res.write(result.dump());
//         return res;
//     }

//     string friendUsername = body["friend_username"].s();

//     try {
//         // Get both user objects
//         User* requester = auth->getUser(requesterUsername);
//         User* friendUser = auth->getUser(friendUsername);

//         if (!requester || !friendUser) {
//             res.code = 404;
//             result["success"] = false;
//             result["message"] = "User not found";
//             res.write(result.dump());
//             return res;
//         }

//         // Check if trying to send request to self
//         if (requesterUsername == friendUsername) {
//             res.code = 400;
//             result["success"] = false;
//             result["message"] = "Cannot send friend request to yourself";
//             res.write(result.dump());
//             return res;
//         }

//         // Send friend request
//         bool success = requester->friendshipmanager->sendRequest(*requester, *friendUser);

//         if (success) {
//             res.code = 200;
//             result["success"] = true;
//             result["message"] = "Friend request sent successfully";
            
//             // Update users in database
//             file dbManager;
//             dbManager.updateUser(*requester);
//             dbManager.updateUser(*friendUser);
//         } else {
//             res.code = 400;
//             result["success"] = false;
//             result["message"] = "Friend request already exists or users are already friends";
//         }
//     } catch (const std::exception& e) {
//         res.code = 500;
//         result["success"] = false;
//         result["message"] = "Internal server error";
//     }

//     res.write(result.dump());
//     return res;
// });

    // // Static file serving for assets
    // CROW_ROUTE(app, "/<string>")
    // ([](const crow::request& req, string filename) {
    //     // Prevent directory traversal
    //     if (filename.find("..") != string::npos) {
    //         return crow::response(403, "Forbidden");
    //     }

    //     // Default to index.html for frontend routes
    //     if (filename.find('.') == string::npos) {
    //         filename = std::filesystem::absolute("static/website_home.html").string();
    //     } else {
    //         filename = std::filesystem::absolute("static/" + filename).string();
    //     }

    //     try {
    //         ifstream file(filename, ios::binary);
    //         if (!file) {
    //             return crow::response(404, "File not found: " + filename);
    //         }

    //         string content((istreambuf_iterator<char>(file)), 
    //                    istreambuf_iterator<char>());
            
    //         crow::response res(content);
    //         res.set_header("Content-Type", getMimeType(filename));
    //         res.set_header("Cache-Control", "public, max-age=3600");
            
    //         // Security headers
    //         res.set_header("X-Content-Type-Options", "nosniff");
            
    //         // Relax CSP for development
    //         if (filename.find(".html") != string::npos) {
    //             res.set_header("Content-Security-Policy", "default-src * 'unsafe-inline' 'unsafe-eval'; style-src * 'unsafe-inline'; img-src * data:; font-src *;");
    //         }
            
    //         return res;
    //     } catch (const exception& e) {
    //         return crow::response(500, string("Error loading file: ") + e.what());
    //     }
    // });


    // Publish post via GET (legacy/simple API)
    CROW_ROUTE(app, "/api/posts/publish").methods("GET"_method)
    ([](const crow::request& req) {
        crow::response res;
        res.set_header("Content-Type", "application/json");

        // Extract query parameters
        auto email = req.url_params.get("email");
        auto content = req.url_params.get("content");
        // if (!email || !content) {
        //     res.code = 400;
        //     res.write(R"({\"success\":false,\"message\":\"Missing email or content\"})");
        //     return res;
        // }

        // Append to posts.csv
        std::ofstream file("../txtFiles/posts.csv", std::ios::app);
        if (!file) {
            std::cerr << "Failed to open posts.csv" << std::endl;
            res.code = 500;
            res.write(R"({\"success\":false,\"message\":\"Failed to open posts.csv\"})");
            return res;
        }
        // Format: id,email,content,likesNo (id is auto-incremented by counting lines)
        int id = 1;
        {
            std::ifstream countFile("../txtFiles/posts.csv");
            std::string line;
            while (std::getline(countFile, line)) ++id;
        }
        // Format: id,email,content,likesNo,comments
        file << id << ',' << email << ',' << '"' << content << '"' << ",0,[]" << "\n";
        file.close();

        res.code = 200;
        res.write(R"({\"success\":true,\"message\":\"Post published\"})");

        return res;
    });

        // Get all posts as JSON
    CROW_ROUTE(app, "/api/posts/get").methods("GET"_method)
    ([](const crow::request& req) {
        crow::response res;
        res.set_header("Content-Type", "application/json");

        std::ifstream file("../txtFiles/posts.csv");
        if (!file) {
            res.code = 500;
            res.write(R"({\"success\":false,\"message\":\"Failed to open posts.csv\"})");
            return res;
        }

        std::string line;
        std::ostringstream json;
        json << "[";
        bool first = true;
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string id, username, content, likes, comments;
            
            // Parse CSV fields
            getline(ss, id, ',');
            getline(ss, username, ',');
            getline(ss, content, ',');
            getline(ss, likes, ',');
            getline(ss, comments); // Get remaining part as comments
            
            // Remove quotes from content if present
            if (!content.empty() && content.front() == '"' && content.back() == '"') {
                content = content.substr(1, content.size() - 2);
            }
            
            // If comments is empty, default to empty array
            if (comments.empty()) {
                comments = "[]";
            }
            
            if (!first) json << ",";
            first = false;
            json << "{";
            json << "\"username\":\"" << username << "\",";
            json << "\"content\":\"" << content << "\",";
            json << "\"likes\":\"" << likes << "\",";
            json << "\"id\":\"" << id << "\",";
            json << "\"comments\":" << comments;
            json << "}";
        }
        json << "]";
        res.code = 200;
        res.write(json.str());
        return res;
    });

    
    // Increment likes for a post
    CROW_ROUTE(app, "/api/posts/like").methods("GET"_method)
    ([](const crow::request& req) {
        crow::response res;
        res.set_header("Content-Type", "application/json");
        crow::json::wvalue result;

        // Get post ID from query parameters
        auto postId = req.url_params.get("id");
        if (!postId) {
            res.code = 400;
            result["success"] = false;
            result["message"] = "Missing post ID";
            res.write(result.dump());
            return res;
        }

        try {
            vector<string> lines;
            bool found = false;
            
            // Read all lines from posts.csv
            std::ifstream inFile("../txtFiles/posts.csv");
            if (!inFile) {
                res.code = 500;
                result["success"] = false;
                result["message"] = "Failed to open posts.csv";
                res.write(result.dump());
                return res;
            }

            string line;
            while (getline(inFile, line)) {
                stringstream ss(line);
                string id, email, content, likes, comments;
                
                // Parse the CSV line
                getline(ss, id, ',');
                getline(ss, email, ',');
                getline(ss, content, ',');
                getline(ss, likes, ',');
                getline(ss, comments); // Get remaining part as comments
                
                if (id == postId) {
                    // Increment likes
                    int likesCount = stoi(likes);
                    likesCount++;
                    // Reconstruct the line with incremented likes, preserving comments
                    lines.push_back(id + "," + email + "," + content + "," + to_string(likesCount) + "," + comments);
                    found = true;
                } else {
                    lines.push_back(line);
                }
            }
            inFile.close();

            if (!found) {
                res.code = 404;
                result["success"] = false;
                result["message"] = "Post not found";
                res.write(result.dump());
                return res;
            }

            // Write back all lines to the file
            std::ofstream outFile("../txtFiles/posts.csv");
            if (!outFile) {
                res.code = 500;
                result["success"] = false;
                result["message"] = "Failed to update posts.csv";
                res.write(result.dump());
                return res;
            }

            for (const auto& updatedLine : lines) {
                outFile << updatedLine << "\n";
            }
            outFile.close();

            res.code = 200;
            result["success"] = true;
            result["message"] = "Like count incremented successfully";
            res.write(result.dump());
            return res;

        } catch (const std::exception& e) {
            res.code = 500;
            result["success"] = false;
            result["message"] = string("Internal server error: ") + e.what();
            res.write(result.dump());
            return res;
        }
    });

    // Add comment to a post
    CROW_ROUTE(app, "/api/posts/comment").methods("GET"_method)
    ([](const crow::request& req) {
        crow::response res;
        res.set_header("Content-Type", "application/json");
        crow::json::wvalue result;

        // Get parameters
        auto postId = req.url_params.get("id");
        auto comment = req.url_params.get("content");
        if (!postId || !comment) {
            res.code = 400;
            result["success"] = false;
            result["message"] = "Missing post ID or comment";
            res.write(result.dump());
            return res;
        }

        try {
            vector<string> lines;
            bool found = false;
            
            // Read all lines from posts.csv
            std::ifstream inFile("../txtFiles/posts.csv");
            if (!inFile) {
                res.code = 500;
                result["success"] = false;
                result["message"] = "Failed to open posts.csv";
                res.write(result.dump());
                return res;
            }

            string line;
            while (getline(inFile, line)) {
                stringstream ss(line);
                string id, email, content, likes, comments;
                
                // Parse the CSV line
                getline(ss, id, ',');
                getline(ss, email, ',');
                getline(ss, content, ',');
                getline(ss, likes, ',');
                getline(ss, comments); // Get remaining part as comments

                if (id == postId) {
                    found = true;
                    // Remove any trailing newlines or whitespace
                    comments = comments.substr(0, comments.find_last_not_of(" \n\r\t") + 1);
                    
                    // Parse existing comments array
                    if (comments.empty() || comments == "[]") {
                        // First comment
                        comments = "[\"" + string(comment) + "\"]";
                    } else {
                        // Add to existing comments
                        // Remove the closing bracket
                        comments = comments.substr(0, comments.length() - 1);
                        if (comments.length() > 1) { // If there are existing comments
                            comments += ",";
                        }
                        comments += "\"" + string(comment) + "\"]";
                    }
                    
                    // Reconstruct the line with new comments
                    lines.push_back(id + "," + email + "," + content + "," + likes + "," + comments);
                } else {
                    lines.push_back(line);
                }
            }
            inFile.close();

            if (!found) {
                res.code = 404;
                result["success"] = false;
                result["message"] = "Post not found";
                res.write(result.dump());
                return res;
            }

            // Write back all lines to the file
            std::ofstream outFile("../txtFiles/posts.csv");
            if (!outFile) {
                res.code = 500;
                result["success"] = false;
                result["message"] = "Failed to update posts.csv";
                res.write(result.dump());
                return res;
            }

            for (const auto& updatedLine : lines) {
                outFile << updatedLine << "\n";
            }
            outFile.close();

            res.code = 200;
            result["success"] = true;
            result["message"] = "Comment added successfully";
            res.write(result.dump());
            return res;

        } catch (const std::exception& e) {
            res.code = 500;
            result["success"] = false;
            result["message"] = string("Internal server error: ") + e.what();
            res.write(result.dump());
            return res;
        }
    });
    

    app.port(18080).multithreaded().run();
}