#include "DatabaseManager.h"
using namespace std;

vector<Post> file:: retrieveData() const{
    vector<Post> posts;
    ifstream txtFile("posts.txt");
    string line,field;
while(getline(txtFile,line)){
    if (line.empty()) continue;
    vector<string>fields;
    istringstream lineStream(line);
    // Split line by pipe character
    while (getline(lineStream, field, '|')) {
        // Trim whitespace from each field
        field.erase(0, field.find_first_not_of(" \t\n\r\f\v"));//left trim
        field.erase(field.find_last_not_of(" \t\n\r\f\v") + 1);//Right
        fields.push_back(field);
    }
    int authorId = stoi(fields[0]);//string to int
    int postId = stoi(fields[1]);
    string author = fields[2];
    string content = fields[3];
    string timestamp = fields[4];
    Post thisPost(authorId, postId, author, content, timestamp);//create obj
    posts.push_back(thisPost);//push to the vector
}
    txtFile.close();
    return posts;
}


void file::insertData(const Post& post){
    ofstream txtFile("posts.txt",ios::app);
    // Write pipe-separated values
    txtFile<< post.authorID << "|"<< post.postID << "|"<< post.author << "|"<< post.content << "|"<< post.timestamp << "\n";
    
    txtFile.close();
}
