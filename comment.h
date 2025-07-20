#include <string>
#include "crow.h"
using namespace std;

class Comment {
public:
    string text;
    string author;
    string timestamp;

    Comment();
    Comment(const string& t, const string& a, const string& ts);

    crow::json::wvalue toJSON() const;
    static Comment fromJSON(const crow::json::rvalue& data);
};