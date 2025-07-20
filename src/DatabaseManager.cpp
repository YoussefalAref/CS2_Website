#include "DatabaseManager.h"

vector<Post> file:: retrieveData() const{
    vector<Post> posts;
    ifstream txtFile("posts.txt");
    string line;
while(getline(txtFile,line)){

}
    txtFile.close();
    return posts;
}
void file::saveData(const Post& p){

}
