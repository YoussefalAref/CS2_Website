#include "DatabaseManager.h"
#include <sstream> 
#include <exception> 
#include <stdexcept>
#include <filesystem>
using namespace std;

//posts:
vector<Post> file:: retrieveData() const{

    vector<Post> posts;

    filesystem::path file_path = filesystem::absolute("../txtFiles/posts.csv");
    ifstream txtFile(file_path);

     if (!txtFile.is_open()) {
        cerr << "Error opening file for reading\n";
        return posts;
    }

    string line;
while(getline(txtFile,line)){
    if (line.empty()) continue;
    vector<string>fields;
    istringstream lineStream(line);
    string field;
    
    while (getline(lineStream, field, '|')) {
        fields.push_back(field);
    }

    if (fields.size() < 5) {
        cerr << "Invalid line format: " << line << endl;
        continue;
    }
    try {
        int authorId = stoi(fields[0]);
        int postId = stoi(fields[1]);
        string author = fields[2];
        string content = fields[3];
        int likesNo = stoi(fields[4]);
        posts.emplace_back(authorId, postId, author, content, likesNo);
    } catch (const exception& e) {
        cerr << "Error processing line: " << line << " - " << e.what() << endl;
    }
    }

    txtFile.close();
    return posts;
}

void file::fillData(const vector<Post>& posts){
    filesystem::path file_path = filesystem::absolute("../textFiles/posts.csv");
    ofstream txtFile(file_path);
    if (!txtFile.is_open()) {
        cerr << "Error opening file for writing\n";
        return;
    }
    for(const Post& po : posts){
        txtFile << po.getAuthorID() << "|"
                << po.getPostID() << "|"
                << po.getAuthor() << "|"
                << po.getContent() << "|"
                << po.getLikesNo() << "\n";
    }
    txtFile.close();
}

void file::insertData(const Post& post){
    filesystem::path file_path = filesystem::absolute("../txtFiles/posts.csv");
    ofstream txtFile(file_path, ios::app);
     if (!txtFile.is_open()) {
        cerr << "Error opening file for appending\n";
        return;
    }

    txtFile << post.getAuthorID() << "|"
            << post.getPostID() << "|"
            << post.getAuthor() << "|"
            << post.getContent() << "|"
            << post.getLikesNo() << "\n";
    
    txtFile.close();
}



//users:
